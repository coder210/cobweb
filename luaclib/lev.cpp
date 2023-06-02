#include "context.h"
#include <cstdlib>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/thread.h>
#include <ctype.h>
#include <mutex>
#include "mylib/network.h"
extern "C" {
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
}


#define CMD_CONNECT 1
#define CMD_MESSAGE 2
#define CMD_DISCONNECT 3
#define CMD_WRITE 4

#define MAX_BUFFER 1024


struct libevent {
	lua_State* L;
	std::mutex mutex_;
	struct evconnlistener* evconn_;
	int callback_;
	long timeout_read_;
	long timeout_write_;
};

static void
_evthread_use_pthreads() {
#ifdef _WIN32
	evthread_use_windows_threads();
#elif _LINUX
	evthread_use_pthreads();
#else
#endif
}



static void
_write_cb(struct bufferevent* bufev, void* arg) {
	libevent* inst = (libevent*)arg;
	inst->mutex_.lock();
	lua_rawgeti(inst->L, LUA_REGISTRYINDEX, inst->callback_);
	lua_pushinteger(inst->L, CMD_WRITE);
	lua_pushlightuserdata(inst->L, bufev);
	if (lua_pcall(inst->L, 2, 0, 0) != 0) {

	}
	inst->mutex_.unlock();
}

static void
_read_cb(struct bufferevent* bufev, void* arg) {
	libevent* inst = (libevent*)arg;
	inst->mutex_.lock();
	lua_rawgeti(inst->L, LUA_REGISTRYINDEX, inst->callback_);
	lua_pushinteger(inst->L, CMD_MESSAGE);
	lua_pushlightuserdata(inst->L, bufev);
	if (lua_pcall(inst->L, 2, 0, 0) != 0) {

	}
	inst->mutex_.unlock();
}

static void
_event_cb(struct bufferevent* bufev, short what, void* arg) {
	if (BEV_EVENT_ERROR & what) {
		goto OFFLINE;
	}
	if (BEV_EVENT_EOF & what) {
		goto OFFLINE;
	}
	if (BEV_EVENT_TIMEOUT & what) {
		goto OFFLINE;
	}
	return;
OFFLINE:
	libevent* inst = (libevent*)arg;
	inst->mutex_.lock();
	lua_rawgeti(inst->L, LUA_REGISTRYINDEX, inst->callback_);
	lua_pushinteger(inst->L, CMD_DISCONNECT);
	lua_pushlightuserdata(inst->L, bufev);
	if (lua_pcall(inst->L, 2, 0, 0) != 0) {

	}
	inst->mutex_.unlock();
}

static void
_accept_cb(struct evconnlistener* listener, evutil_socket_t clientfd,
	struct sockaddr* addr, int len, void* arg) {
	libevent* inst = (libevent*)arg;
	struct event_base* base = evconnlistener_get_base(listener);
	evutil_make_socket_nonblocking(clientfd);
	struct bufferevent* bufev = bufferevent_socket_new(base, clientfd, BEV_OPT_THREADSAFE | BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
	bufferevent_setcb(bufev, (bufferevent_data_cb)_read_cb, (bufferevent_data_cb)_write_cb, (bufferevent_event_cb)_event_cb, arg);
	struct timeval timeout_read = { inst->timeout_read_, 0 };
	struct timeval timeout_write = { inst->timeout_write_, 0 };
	bufferevent_set_timeouts(bufev, &timeout_read, &timeout_write);
	bufferevent_setwatermark(bufev, EV_READ, 2, 0);
	bufferevent_setwatermark(bufev, EV_WRITE, 2, 0);
	bufferevent_enable(bufev, EV_READ | EV_WRITE);
	struct sockaddr_in* addr_in = (struct sockaddr_in*)addr;
	const char* ip = inet_ntoa(addr_in->sin_addr);
	int port = ntohs(addr_in->sin_port);
	inst->mutex_.lock();
	lua_rawgeti(inst->L, LUA_REGISTRYINDEX, inst->callback_);
	lua_pushinteger(inst->L, CMD_CONNECT);
	lua_pushlightuserdata(inst->L, bufev);
	lua_pushstring(inst->L, ip);
	lua_pushinteger(inst->L, port);
	if (lua_pcall(inst->L, 4, 0, 0) != 0) {

	}
	inst->mutex_.unlock();
}

static int
levent_pack(lua_State* L) {
	int stype = luaL_checkinteger(L, 1);
	void* data = lua_touserdata(L, 2);
	SocketMessage* socket_message = new SocketMessage();
	socket_message->stype_ = stype;
	socket_message->data_ = data;
	lua_pushlightuserdata(L, socket_message);
	return 1;
}

static int
levent_unpack(lua_State* L) {
	SocketMessage* socket_message = (SocketMessage*)lua_touserdata(L, 1);
	lua_pushinteger(L, socket_message->stype_);
	lua_pushlightuserdata(L, socket_message->data_);
	return 2;
}

static int
levent_free(lua_State* L) {
	SocketMessage* socket_message = (SocketMessage*)lua_touserdata(L, 1);
	delete socket_message;
	return 0;
}

static int
levent_create(lua_State* L) {
	_evthread_use_pthreads();
	libevent* inst =  new libevent();
	if (inst != nullptr) {
		lua_pushlightuserdata(L, inst);
	}
	else {
		lua_pushnil(L);
	}
	return 1;
}

static int
levent_bind(lua_State* L) {
	libevent* inst = (libevent*)lua_touserdata(L, 1);
	const char* ip = luaL_checkstring(L, 2);
	int port = (int)luaL_checkinteger(L, 3);
	inst->callback_ = luaL_ref(L, LUA_REGISTRYINDEX);
	inst->L = L;
	struct event_base* base = event_base_new();
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(port);
	struct evconnlistener* evconn = evconnlistener_new_bind(base, _accept_cb, inst, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, 512, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));
	inst->evconn_ = evconn;
	lua_pushboolean(L, inst->evconn_ != NULL ? true : false);
	return 1;
}

static int
levent_set_timeouts(lua_State* L) {
	libevent* inst = (libevent*)lua_touserdata(L, 1);
	inst->timeout_read_ = (long)luaL_checkinteger(L, 2);
	inst->timeout_write_ = (long)luaL_checkinteger(L, 3);
	return 0;
}

static int
levent_base_loop(lua_State* L) {
	libevent* inst = (libevent*)lua_touserdata(L, 1);
	int flag = (int)luaL_checkinteger(L, 2);
	struct event_base* base = evconnlistener_get_base(inst->evconn_);
	int ret = event_base_loop(base, flag);
	lua_pushinteger(L, ret);
	return 1;
}

static int
levent_base_release(lua_State* L) {
	libevent* inst = (libevent*)lua_touserdata(L, 1);
	bool ret = false;
	if (inst != NULL) {
		struct event_base* base = evconnlistener_get_base(inst->evconn_);
		evconnlistener_free(inst->evconn_);
		event_base_free(base);
		delete inst;
		ret = true;
	}
	lua_pushboolean(L, ret);
	return 1;
}

static int
lbufferevent_get_length(lua_State* L) {
	struct bufferevent* bufev = (struct bufferevent*)lua_touserdata(L, 1);
	struct evbuffer* input_evbuffer = bufferevent_get_input(bufev);
	size_t sz = evbuffer_get_length(input_evbuffer);
	lua_pushinteger(L, sz);
	return 1;
}

static int
lbufferevent_remove(lua_State* L) {
	struct bufferevent* bufev = (struct bufferevent*)lua_touserdata(L, 1);
	size_t datlen = (size_t)luaL_checkinteger(L, 2);
	char buffer[MAX_BUFFER] = { 0 };
	struct evbuffer* input_evbuffer = bufferevent_get_input(bufev);
	int ret = evbuffer_remove(input_evbuffer, buffer, datlen);
	if (ret != -1) {
		lua_pushlstring(L, buffer, ret);
		lua_pushinteger(L, ret);
	}
	else {
		lua_pushnil(L);
		lua_pushinteger(L, 0);
	}
	return 2;
}

static int
lbufferevent_copyout(lua_State* L) {
	bufferevent* bufev = (bufferevent*)lua_touserdata(L, 1);
	size_t len = (size_t)luaL_checkinteger(L, 2);
	struct evbuffer* input_evbuffer = bufferevent_get_input(bufev);
	char buffer[MAX_BUFFER] = { 0 };
	int ret = evbuffer_copyout(input_evbuffer, buffer, len);
	if (ret != -1) {
		lua_pushlstring(L, buffer, ret);
	}
	else {
		lua_pushnil(L);
	}
	return 1;
}

static int
lbufferevent_drain(lua_State* L) {
	struct bufferevent* bufev = (struct bufferevent*)lua_touserdata(L, 1);
	size_t len = (size_t)luaL_checkinteger(L, 2);
	struct evbuffer* input_evbuffer = bufferevent_get_input(bufev);
	int ret = evbuffer_drain(input_evbuffer, len);
	lua_pushboolean(L, ret == 0);
	return 1;
}

static int
lbufferevent_search(lua_State* L) {
	struct bufferevent* bufev = (struct bufferevent*)lua_touserdata(L, 1);
	size_t len;
	const char* what = lua_tolstring(L, 2, &len);
	int pos = (int)luaL_checkinteger(L, 3);
	struct evbuffer* input_evbuffer = bufferevent_get_input(bufev);
	evbuffer_ptr begin = { pos = pos };
	evbuffer_ptr ret = evbuffer_search(input_evbuffer, what, len, &begin);
	lua_pushinteger(L, ret.pos);
	return 1;
}

static int
lbufferevent_getfd(lua_State* L) {
	struct bufferevent* bufev = (struct bufferevent*)lua_touserdata(L, 1);
	int fd = bufferevent_getfd(bufev);
	lua_pushinteger(L, fd);
	return 1;
}

static int
lbufferevent_write(lua_State* L) {
	struct bufferevent* bufev = (struct bufferevent*)lua_touserdata(L, 1);
	void* data = NULL;
	if (lua_isstring(L, 2)) {
		data = (void*)luaL_checkstring(L, 2);
	}
	else {
		data = lua_touserdata(L, 2);
	}
	size_t len = (size_t)luaL_checkinteger(L, 3);
	int ret = bufferevent_write(bufev, data, len);
	lua_pushboolean(L, ret == 0);
	return 1;
}

static int
lbufferevent_free(lua_State* L) {
	struct bufferevent* bufev = (struct bufferevent*)lua_touserdata(L, 1);
	bufferevent_free(bufev);
	return 0;
}

COBWEB_MOD_API int
luaopen_lev(lua_State* L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{"pack", levent_pack},
		{"unpack", levent_unpack},
		{"free", levent_free},
		{"create", levent_create},
		{"set_timeouts", levent_set_timeouts},
		{"bind", levent_bind},
		{"loop", levent_base_loop},
		{"release", levent_base_release},
		{"bufferevent_get_length", lbufferevent_get_length},
		{"bufferevent_remove", lbufferevent_remove},
		{"bufferevent_copyout", lbufferevent_copyout},
		{"bufferevent_drain", lbufferevent_drain},
		{"bufferevent_search", lbufferevent_search},
		{"bufferevent_getfd", lbufferevent_getfd},
		{"bufferevent_write", lbufferevent_write},
		{"bufferevent_free", lbufferevent_free},
		{NULL, NULL}
	};
	luaL_newlib(L, l);
	return 1;
}
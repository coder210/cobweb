#include <stdint.h>
#include <memory.h>
#include <malloc.h>
#include "mylib/network.h"
#include "context.h"
extern "C" {
#include "lua/lapi.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

#define MAX_IP 128 
#define MESSAGE_SIZE 1024

short
_read_short(uint8_t* buffer) {
	short r = (short)buffer[0] << 8 | (short)buffer[1];
	return r;
}

static void
_write_short(uint8_t* buffer, short value) {
	buffer[0] = (value >> 8) & 0xff;
	buffer[1] = value & 0xff;
}

/*
* 对应c# = BitConverter.ToUInt32
*/
static void
_write_int(uint8_t* buffer, int value) {
	buffer[3] = ((0xff000000 & value) >> 24);
	buffer[2] = ((0xff0000 & value) >> 16);
	buffer[1] = ((0xff00 & value) >> 8);
	buffer[0] = (0xff & value);
}

/*
* 对应c# = BitConverter.ToUInt32
*/
static int
_read_int(uint8_t* buffer) {
	int r = buffer[0];
	r |= (int)buffer[1] << 8;
	r |= (int)buffer[2] << 16;
	r |= (int)buffer[3] << 24;
	return r;
}

static const char*
tolstring(lua_State* L, size_t* sz, int index) {
	const char* ptr;
	if (lua_isuserdata(L, index)) {
		ptr = (const char*)lua_touserdata(L, index);
		*sz = (size_t)(int)luaL_checkinteger(L, index + 1);
	}
	else {
		ptr = luaL_checklstring(L, index, sz);
	}
	return ptr;
}

static int
lnetpack_pack(struct lua_State* L) {
	size_t len;
	const char* ptr = tolstring(L, &len, 1);
	if (len >= 0x10000) {
		return luaL_error(L, "Invalid size (too long) of data : %d", (int)len);
	}

	uint8_t* buffer = (uint8_t*)malloc(len + 2);
	_write_short(buffer, len);
	memcpy(buffer + 2, ptr, len);

	lua_pushlightuserdata(L, buffer);
	lua_pushinteger(L, len + 2);

	return 2;
}

static int
lnetpack_unpack(struct lua_State* L) {
	size_t len = 0;
	const char* ptr = luaL_checklstring(L, 1, &len);
	short size = ptr[1] & 0x000000FF;
	size |= ((ptr[0] << 8) & 0x0000FF00);
	lua_pushinteger(L, size);
	return 1;
}

static int
lnetpack_pack32(struct lua_State* L) {
	size_t len;
	const char* ptr = tolstring(L, &len, 1);
	if (len >= 0x100000000) {
		return luaL_error(L, "Invalid size (too long) of data : %d", len);
	}

	uint8_t* buffer = (uint8_t*)malloc(len + 4);
	_write_int(buffer, len);
	memcpy(buffer + 4, ptr, len);

	lua_pushlightuserdata(L, buffer);
	lua_pushinteger(L, len + 4);

	return 2;
}

static int
lnetpack_unpack32(struct lua_State* L) {
	size_t len = 0;
	const char* ptr = luaL_checklstring(L, 1, &len);
	int size = _read_int((uint8_t*)ptr);
	lua_pushinteger(L, size);
	return 1;
}

static int
lnetpack_free(struct lua_State* L) {
	void* ptr = lua_touserdata(L, 1);
	if (ptr != NULL) {
		free(ptr);
	}
	return 0;
}

static int
lnetpack_tostring(lua_State* L) {
	void* ptr = lua_touserdata(L, 1);
	int size = (int)luaL_checkinteger(L, 2);
	if (ptr == NULL) {
		lua_pushliteral(L, "");
	}
	else {
		lua_pushlstring(L, (const char*)ptr, size);
		free(ptr);
	}
	return 1;
}

static int
lsocket_init(struct lua_State* L) {
	bool ret = NetworkSystem::init();
	lua_pushboolean(L, ret);
	return 1;
}

static int
lsocket_release(struct lua_State* L) {
	bool ret = NetworkSystem::release();
	lua_pushboolean(L, ret);
	return 1;
}

static int
lsocket_bind(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	const char* ip = luaL_checkstring(L, 2);
	int port = (int)luaL_checkinteger(L, 3);
	if (NetworkSystem::socket_bind(sockfd, ip, port)) {
		lua_pushboolean(L, true);
	}
	else {
		lua_pushboolean(L, false);
	}
	return 1;
}

static int
lsocket_nonblock(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	bool result = NetworkSystem::socket_nonblock(sockfd);
	lua_pushboolean(L, result);
	return 1;
}

static int
lsocket_close(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	if (NetworkSystem::socket_close(sockfd)) {
		lua_pushboolean(L, true);
		lua_pushstring(L, "success");
	}
	else {
		lua_pushboolean(L, false);
		lua_pushstring(L, "fail");
	}
	return 2;
}

static int
ltcp_create(struct lua_State* L) {
	int sockfd = NetworkSystem::tcp_create();
	lua_pushinteger(L, sockfd);
	return 1;
}

static int
ltcp_connect(struct lua_State* L) {
	int sockfd = (int)lua_tointeger(L, 1);
	const char* ip = luaL_checkstring(L, 2);
	int port = (int)luaL_checkinteger(L, 3);
	if (NetworkSystem::tcp_connect(sockfd, ip, port)) {
		lua_pushboolean(L, true);
		lua_pushstring(L, "success");
	}
	else {
		lua_pushboolean(L, false);
		lua_pushstring(L, "fail");
	}
	return 2;
}

static int
ltcp_listen(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	if (NetworkSystem::tcp_listen(sockfd)) {
		lua_pushboolean(L, true);
		lua_pushstring(L, "success");
	}
	else {
		lua_pushboolean(L, false);
		lua_pushstring(L, "fail");
	}
	return 2;
}

static int
ltcp_accept(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	struct sockaddr_t sockaddr = NetworkSystem::tcp_accept(sockfd);
	lua_pushinteger(L, sockaddr.sockfd);
	lua_pushstring(L, sockaddr.ip);
	lua_pushinteger(L, sockaddr.port);
	return 3;
}

static int
ltcp_recv(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	unsigned char buf[MESSAGE_SIZE];
	memset(buf, 0, MESSAGE_SIZE);
	int len = NetworkSystem::tcp_recv(sockfd, (char*)buf);
	if (len > 0) {
		lua_pushinteger(L, len);
		lua_pushlstring(L, (char*)buf, len);
	}
	else {
		lua_pushinteger(L, len);
		lua_pushnil(L);
	}

	return 2;
}

static int
ltcp_recvb(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	unsigned char buf[MESSAGE_SIZE];
	memset(buf, 0, MESSAGE_SIZE);
	int len = NetworkSystem::tcp_noblock_recv(sockfd, (char*)buf);
	if (len > 0) {
		lua_pushinteger(L, len);
		lua_pushlstring(L, (char*)buf, len);
	}
	else {
		lua_pushinteger(L, len);
		lua_pushnil(L);
	}

	return 2;
}

static int
ltcp_send(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	size_t len = 0;
	const char* ptr = luaL_checklstring(L, 2, &len);
	int n = NetworkSystem::tcp_send(sockfd, ptr, len);
	lua_pushinteger(L, n);
	return 1;
}

static int
ludp_create(struct lua_State* L) {
	int sockfd = NetworkSystem::udp_create();
	lua_pushinteger(L, sockfd);
	return 1;
}

static int
ludp_recvfrom(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	char buffer[MESSAGE_SIZE] = { 0 };
	char ip[MAX_IP] = { 0 };
	int port = 0;
	int n = NetworkSystem::udp_recv(sockfd, buffer, ip, &port);
	lua_pushboolean(L, n != -1);
	lua_newtable(L);
	if (n != -1) {
		lua_pushstring(L, "buffer");
		lua_pushlstring(L, buffer, n);
		lua_settable(L, -3);
	}
	else {
		lua_pushstring(L, "buffer");
		lua_pushstring(L, strerror(n));
		lua_settable(L, -3);
	}
	lua_pushstring(L, "len");
	lua_pushinteger(L, n);
	lua_settable(L, -3);
	lua_pushstring(L, "ip");
	lua_pushstring(L, ip);
	lua_settable(L, -3);
	lua_pushstring(L, "port");
	lua_pushinteger(L, port);
	lua_settable(L, -3);
	return 2;
}

static int
ludp_recvfromx(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	char buffer[MESSAGE_SIZE] = { 0 };
	char ip[MAX_IP] = { 0 };
	int port = 0;
	int n = NetworkSystem::udp_recv(sockfd, buffer, ip, &port);
	lua_pushboolean(L, n != -1);
	lua_newtable(L);
	if (n != -1) {
		lua_pushstring(L, "buffer");
		lua_pushlstring(L, buffer, n);
		lua_settable(L, -3);
	}
	else {
		lua_pushstring(L, "buffer");
		lua_pushstring(L, strerror(n));
		lua_settable(L, -3);
	}
	lua_pushstring(L, "len");
	lua_pushinteger(L, n);
	lua_settable(L, -3);
	lua_pushstring(L, "ip");
	lua_pushstring(L, ip);
	lua_settable(L, -3);
	lua_pushstring(L, "port");
	lua_pushinteger(L, port);
	lua_settable(L, -3);
	return 2;
}

static int
ludp_sendto(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	const char* buffer = luaL_checkstring(L, 2);
	int len = (int)luaL_checkinteger(L, 3);
	const char* ip = luaL_checkstring(L, 4);
	int port = (int)luaL_checkinteger(L, 5);
	int n = NetworkSystem::udp_send(sockfd, buffer, len, ip, port);
	lua_pushinteger(L, n);
	return 1;
}

static int
ludp_sendtox(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	size_t len = 0;
	const char* buffer = luaL_checklstring(L, 2, &len);
	const char* ip = luaL_checkstring(L, 3);
	int port = (int)luaL_checkinteger(L, 4);
	int n = NetworkSystem::udp_send(sockfd, buffer, len, ip, port);
	lua_pushinteger(L, n);
	lua_pushinteger(L, n);
	return 1;
}

COBWEB_MOD_API int
luaopen_lnetwork(lua_State* L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{"pack", lnetpack_pack},
		{"unpack", lnetpack_unpack},
		{"pack32", lnetpack_pack32},
		{"unpack32", lnetpack_unpack32},
		{"tostring", lnetpack_tostring},
		{"free", lnetpack_free},
		{"init", lsocket_init},
		{"release", lsocket_release},
		{"close", lsocket_close},
		{"bind", lsocket_bind},
		{"nonblock", lsocket_nonblock},
		{"tcp", ltcp_create},
		{"connect", ltcp_connect},
		{"listen", ltcp_listen},
		{"accept", ltcp_accept},
		{"send", ltcp_send},
		{"recv", ltcp_recv},
		{"recvb", ltcp_recvb},
		{"udp", ludp_create},
		{"recvfrom", ludp_recvfrom},
		{"recvfromx", ludp_recvfromx},
		{"sendto", ludp_sendto},
		{"sendtox", ludp_sendtox},
		{NULL, NULL}
	};
	luaL_newlib(L, l);
	return 1;
}

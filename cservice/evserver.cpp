/***************************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: evserver.cpp
Description: evserver
Author: ydlc
Version: 1.0
Date: 2021.12.10
History:
****************************************************/

#include <string>
#include <map>
#include <vector>
#include <cstdlib>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/thread.h>
#include <ctype.h>
#include <mutex>
#include "context.h"
#include "mylib/network.h"

#define MAX_BUFFER 1024
#define EVSERVER_DISPATCH 10 

struct evserver {
	struct evconnlistener* evconn_;
	Context* ctx_;
	std::string dest_;
	int timeout_read_;
	int timeout_write_;
};

static void
_split_string(std::string str, const char split, std::vector<std::string>& substr_vector) {
	int index = -1;
	while ((index = str.find(split)) != -1) {
		std::string substr = str.substr(0, index);
		substr_vector.push_back(substr);
		str = str.substr(index + 1);
	}
	if (substr_vector.size() > 0) {
		substr_vector.push_back(str);
	}
}

static void
_evthread_use_pthreads() {
#ifdef _WIN32
	evthread_use_windows_threads();
#elif _LINUX
	evthread_use_pthreads();
#else
#endif
}

COBWEB_MOD_API evserver*
evserver_create(void) {
	_evthread_use_pthreads();
	NetworkSystem::init();
	evserver* inst = new evserver();
	inst->dest_ = "";
	inst->ctx_ = nullptr;
	inst->timeout_read_ = 120;
	inst->timeout_write_ = 0;
	return inst;
}

COBWEB_MOD_API void
evserver_release(evserver* inst) {
	struct event_base* base = evconnlistener_get_base(inst->evconn_);
	evconnlistener_free(inst->evconn_);
	event_base_free(base);
	delete inst;
	NetworkSystem::release();
}

static bool
evserver_cb(Context* ctx, void* ud, int type, int session, uint32_t source, const void* data, size_t sz) {
	evserver* inst = (evserver*)ud;
	if (type == EVSERVER_DISPATCH) {
		struct event_base* base = evconnlistener_get_base(inst->evconn_);
		int ret = event_base_loop(base, EVLOOP_NONBLOCK);
		inst->ctx_->timeout(inst->ctx_->get_handle(inst->ctx_), EVSERVER_DISPATCH, 0, nullptr, 0, 1);
	}
	return true;
}

static void
_write_cb(struct bufferevent* bufev, void* arg) {
	evserver* inst = (evserver*)arg;
}

static void
_read_cb(struct bufferevent* bufev, void* arg) {
	evserver* inst = (evserver*)arg;
	uint32_t source = inst->ctx_->get_handle(inst->ctx_);
	SocketMessage socket_message;
	socket_message.stype_ = SOCKET_TYPE_DATA;
	socket_message.data_ = bufev;
	inst->ctx_->sendname(source, inst->dest_, PTYPE_SOCKET | PTYPE_TAG_USERDATA, 0, &socket_message, sizeof(socket_message));
}

static void
_event_cb(struct bufferevent* buffev, short what, void* arg) {
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
	evserver* inst = (evserver*)arg;
	uint32_t source = inst->ctx_->get_handle(inst->ctx_);
	SocketMessage socket_message;
	socket_message.stype_ = SOCKET_TYPE_CLOSE;
	socket_message.data_ = buffev;
	inst->ctx_->sendname(source, inst->dest_, PTYPE_SOCKET | PTYPE_TAG_USERDATA, 0, &socket_message, sizeof(socket_message));
}

static void
_accept_cb(struct evconnlistener* listener, evutil_socket_t clientfd,
	struct sockaddr* addr, int len, void* arg) {
	struct evserver* inst = (evserver*)arg;
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
	uint32_t source = inst->ctx_->get_handle(inst->ctx_);
	SocketMessage socket_message;
	socket_message.stype_ = SOCKET_TYPE_CONNECT;
	socket_message.data_ = bufev;
	inst->ctx_->sendname(source, inst->dest_, PTYPE_SOCKET | PTYPE_TAG_USERDATA, 0, &socket_message, sizeof(socket_message));
}


COBWEB_MOD_API bool
evserver_init(evserver* inst, Context* ctx, std::string param) {
	std::vector<std::string> substr_vector;
	_split_string(param, ':', substr_vector);
	if (substr_vector.size() <= 0) {
		return false;
	}
	std::string ip = substr_vector.at(0);
	int port = std::stoi(substr_vector.at(1));
	inst->dest_ = substr_vector.at(2);
	struct event_base* base = event_base_new();
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
	server_addr.sin_port = htons(port);
	struct evconnlistener* evconn = evconnlistener_new_bind(base, _accept_cb, inst, LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, 512, (struct sockaddr*)&server_addr, sizeof(struct sockaddr_in));
	inst->evconn_ = evconn;
	inst->ctx_ = ctx;
	inst->ctx_->callback(inst->ctx_, inst, evserver_cb);
	ctx->command(ctx, "REG", ".evserver");

	inst->ctx_->timeout(inst->ctx_->get_handle(inst->ctx_), EVSERVER_DISPATCH, 0, nullptr, 0, 1);

	return true;
}

COBWEB_MOD_API void
evserver_signal(evserver* inst, int signal) {

}

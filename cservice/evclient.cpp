/***************************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: evclient.cpp
Description: evclient
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
#include "mylib\network.h"

#define MAX_BUFFER 1024
#define EVCLIENT_DISPATCH 10 

struct evclient {
	int fd_;
	std::string ip_;
	int port_;
	Context* ctx_;
	struct bufferevent* buffev_;
	std::string dest_;
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

COBWEB_MOD_API evclient*
evclient_create(void) {
	_evthread_use_pthreads();
	NetworkSystem::init();
	evclient* inst = new evclient();
	inst->buffev_ = NULL;
	inst->dest_ = "";
	inst->ctx_ = nullptr;
	return inst;
}

COBWEB_MOD_API void
evclient_release(evclient* inst) {
	event_base_free(bufferevent_get_base(inst->buffev_));
	bufferevent_free(inst->buffev_);
	NetworkSystem::release();
	delete inst;
}

static bool
evclient_cb(Context* ctx, void* ud, int type, int session, uint32_t source, const void* data, size_t sz) {
	evclient* inst = (evclient*)ud;
	if (type == EVCLIENT_DISPATCH) {
		int ret = event_base_loop(bufferevent_get_base(inst->buffev_), EVLOOP_NONBLOCK);
		inst->ctx_->timeout(inst->ctx_->get_handle(inst->ctx_), EVCLIENT_DISPATCH, 0, nullptr, 0, 1);
	}
	return true;
}

static void 
_read_cb(struct bufferevent* bufev, void* arg) {
	evclient* inst = (evclient*)arg;
	uint32_t source = inst->ctx_->get_handle(inst->ctx_);
	SocketMessage socket_message;
	socket_message.stype_ = SOCKET_TYPE_DATA;
	socket_message.data_ = bufev;
	inst->ctx_->sendname(source, inst->dest_, PTYPE_SOCKET | PTYPE_TAG_USERDATA, 0, &socket_message, sizeof(socket_message));
}

static void 
_write_cb(struct bufferevent* buffev, void* arg) {
	evclient* inst = (evclient*)arg;
}

static void
_event_cb(struct bufferevent* buffev, short events, void* arg) {
	evclient* inst = (evclient*)arg;
	if ((events & BEV_EVENT_ERROR) || (events & BEV_EVENT_EOF)) {
		uint32_t source = inst->ctx_->get_handle(inst->ctx_);
		SocketMessage socket_message;
		socket_message.stype_ = SOCKET_TYPE_ERROR;
		socket_message.data_ = buffev;
		inst->ctx_->sendname(source, inst->dest_, PTYPE_SOCKET | PTYPE_TAG_USERDATA, 0, &socket_message, sizeof(socket_message));
	}
	else if (events & BEV_EVENT_CONNECTED) {
		uint32_t source = inst->ctx_->get_handle(inst->ctx_);
		SocketMessage socket_message;
		socket_message.stype_ = SOCKET_TYPE_CONNECT;
		socket_message.data_ = buffev;
		inst->ctx_->sendname(source, inst->dest_, PTYPE_SOCKET | PTYPE_TAG_USERDATA, 0, &socket_message, sizeof(socket_message));
	}
}

COBWEB_MOD_API bool
evclient_init(evclient* inst, Context* ctx, std::string param) {
	std::vector<std::string> substr_vector;
	_split_string(param, ':', substr_vector);
	if (substr_vector.size() <= 0) {
		return false;
	}
	inst->ip_ = substr_vector.at(0);
	inst->port_ = std::stoi(substr_vector.at(1));
	inst->dest_ = substr_vector.at(2);

	struct event_base* base = event_base_new();
	struct bufferevent* buffev = bufferevent_socket_new(base, -1, BEV_OPT_THREADSAFE | BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(inst->ip_.c_str());
	server_addr.sin_port = htons(inst->port_);
	int ret = bufferevent_socket_connect(buffev, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if (ret != 0) {
		return false;
	}

	bufferevent_setcb(buffev, _read_cb, _write_cb, _event_cb, inst);
	bufferevent_enable(buffev, EV_READ | EV_WRITE);
	bufferevent_setwatermark(buffev, EV_READ, 2, 0);
	bufferevent_setwatermark(buffev, EV_WRITE, 2, 0);

	inst->buffev_ = buffev;
	inst->ctx_ = ctx;
	inst->ctx_->callback(inst->ctx_, inst, evclient_cb);
	ctx->command(ctx, "REG", ".evclient");

	inst->ctx_->timeout(inst->ctx_->get_handle(inst->ctx_), EVCLIENT_DISPATCH, 0, nullptr, 0, 1);

	return true;
}

COBWEB_MOD_API void
evclient_signal(evclient* inst, int signal) {

}

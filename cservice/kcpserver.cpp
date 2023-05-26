/***************************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: kcpserver.cpp
Description: kcpserver
Author: ydlc
Version: 1.0
Date: 2021.12.10
History:
****************************************************/

#include <string>
#include <map>
#include "kcp/kcp.h"
#include "mylib/network.h"
#include "cobweb.h"


#define MAX_BUFFER 1024
#define MAX_IP 128
#define DEFAULT_HEART 3


static inline IINT64
_iclock64(void) {
	long s, u;
	IINT64 value;
	value = ((IINT64)s) * 1000 + (u / 1000);
	return value;
}

static inline IUINT32
_iclock() {
	return (IUINT32)(_iclock64() & 0xfffffffful);
}

/* 对应c# = BitConverter.ToUInt32 */
static void
_write_int(uint8_t* buffer, int value) {
	buffer[3] = ((0xff000000 & value) >> 24);
	buffer[2] = ((0xff0000 & value) >> 16);
	buffer[1] = ((0xff00 & value) >> 8);
	buffer[0] = (0xff & value);
}

/* 对应c# = BitConverter.ToUInt32 */
static int
_read_int(uint8_t* buffer) {
	int r = buffer[0];
	r |= (int)buffer[1] << 8;
	r |= (int)buffer[2] << 16;
	r |= (int)buffer[3] << 24;
	return r;
}

struct node {
	int conv_;
	ikcpcb* kcpcb_;
	int sockfd_;
	std::string ip_;
	int port_;
	int heart_; // 心跳<=0时表示掉线
	uint64_t died_timestamp_; // 时间戳
};

struct kcpserver {
	int server_fd_;
	int id_;
	size_t timeout_;
	std::map<int, node*> clients_;
	bool done_;
};


COBWEB_MOD_API kcpserver*
kcpserver_create(void) {
	NetworkSystem::init();
	kcpserver* inst = new kcpserver();
	inst->id_ = 300;
	inst->server_fd_ = NetworkSystem::udp_create();
	inst->done_ = true;
	inst->timeout_ = 120;
	return inst;
}

COBWEB_MOD_API void
kcpserver_release(kcpserver* inst) {
	NetworkSystem::socket_close(inst->server_fd_);
	NetworkSystem::release();
	delete inst;
}

static int
_output(const char* data, int size,
	ikcpcb* kcpcb, void* user) {
	int ret = 0;
	node* client = (node*)user;
	if (client != nullptr) {
		ret = NetworkSystem::udp_send(client->sockfd_, data, size, client->ip_.c_str(), client->port_);
	}
	return ret;
}

node*
_new(Context* ctx, int fd, int server_fd, const char* ip, int port) {
	node* client = new node();
	if (client != nullptr) {
		client->conv_ = fd;
		client->kcpcb_ = ikcp_create(client->conv_, client);
		client->sockfd_ = server_fd;
		client->ip_ = ip;
		client->port_ = port;
		client->heart_ = DEFAULT_HEART;
		client->died_timestamp_ = ctx->timestamp();
		ikcp_wndsize(client->kcpcb_, 512, 512);
		ikcp_setoutput(client->kcpcb_, _output);
	}
	return client;
}


static bool
_update(Context* ctx, void* data, void* ptr) {
	node* client = (node*)data;
	kcpserver* inst = (kcpserver*)ptr;
	if (client->heart_ > 0) {
		uint64_t curr_timestamp = ctx->timestamp();
		uint64_t ms = inst->timeout_ * 1000; // 换算成毫秒
		if (client->died_timestamp_ + ms <= curr_timestamp) {
			client->died_timestamp_ = curr_timestamp;
			client->heart_--;
		}
		ikcp_update(client->kcpcb_, ikcp_check(client->kcpcb_, _iclock()));
	}
	return 0;
}

static bool
_clear(Context* ctx, void* data, void* ptr) {
	node* client = (node*)data;
	if (client->heart_ <= 0) {
		/*  */
		ctx->log(ctx->get_handle(ctx), "conv=%d disconnect", client->conv_);
		ikcp_release(client->kcpcb_);
		return true;
	}
	else {
		return false;
	}
}

static void
_recv(Context* ctx, kcpserver* inst, int conv, const char* buffer, int len) {
	node* client = inst->clients_.at(conv);
	if (client == nullptr) {
		return;
	}
	ikcp_input(client->kcpcb_, buffer, len);
	while (true) {
		int peeksize = ikcp_peeksize(client->kcpcb_);
		if (peeksize <= 0) {
			break;
		}
		if (peeksize > MAX_BUFFER) {
			break;
		}
		client->died_timestamp_ = ctx->timestamp();
		char* buffer = new char[peeksize];
		if (ikcp_recv(client->kcpcb_, buffer, peeksize) > 0) {
			client->heart_ = DEFAULT_HEART;
			/* send message */

			delete[] buffer;
		}
	}
}

static void
_accept(Context* ctx, kcpserver* inst, const char* ip, int port) {
	inst->id_++;
	node* client = _new(ctx, inst->id_, inst->server_fd_, ip, port);
	if (client == nullptr) {
		ctx->log(ctx->get_handle(ctx), "creating client is failed!");
		return;
	}

	/* insert */
	inst->clients_.insert(std::map<int, node*>::value_type(client->conv_, client));
	uint8_t buffer[4] = { 0 };
	_write_int(buffer, client->conv_);
	NetworkSystem::udp_send(client->sockfd_, (char*)buffer, 4, ip, port);

	/* accept */



}

static void
_dispatch(Context* ctx, kcpserver* inst) {
	char ip[MAX_IP] = { 0 };
	int port = 0;
	char buffer[MAX_BUFFER] = { 0 };
	int ret = NetworkSystem::udp_recv(inst->server_fd_, buffer, ip, &port);
	if (ret != -1 && ret >= 2) {
		int conv = _read_int((uint8_t*)buffer);
		if (conv == 0) {
			_accept(ctx, inst, ip, port);
		}
		else {
			_recv(ctx, inst, conv, buffer, ret);
		}
	}
}


static bool
kcpserver_cb(Context* ctx, void* ud, int type, int session, uint32_t source, const void* data, size_t sz) {
	kcpserver* inst = (struct kcpserver*)ud;
	if (type == PTYPE_RESPONSE) {
		//if (data.compare("timeout") == 0 && inst->done_) {

		//	/* 分发 */
		//	_dispatch(ctx, inst);

		//	/* 定时器 */
		//	std::string str("timeout");
		//	ctx->timeout(ctx->get_handle(ctx), 0, str.c_str(), str.size(), 10);
		//}
	}

	return true;
}

/*
param: ip:port
*/
COBWEB_MOD_API bool
kcpserver_init(kcpserver* inst, Context* ctx, std::string param) {
	int index = param.find(':');
	if (index == -1) {
		return false;
	}
	std::string ip = param.substr(0, index);
	int port = std::stoi(param.substr(index + 1));
	if (!NetworkSystem::socket_bind(inst->server_fd_, ip.c_str(), port)) {
		return false;
	}
	NetworkSystem::socket_nonblock(inst->server_fd_);
	ctx->callback(ctx, inst, kcpserver_cb);
	ctx->command(ctx, "REG", ".kcpserver");
	std::string data("timeout");
	ctx->timeout(ctx->get_handle(ctx), PTYPE_RESPONSE | PTYPE_TAG_STRING, 0, data.c_str(), data.size(), 10);
	return true;
}

COBWEB_MOD_API void
kcpserver_signal(kcpserver* inst, int signal) {

}

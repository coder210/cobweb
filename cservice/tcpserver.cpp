/***************************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: tcpserver.cpp
Description: tcpserver
Author: ydlc
Version: 1.0
Date: 2021.12.10
History:
****************************************************/

#include <string>
#include <map>
#include "cobweb.h"
#include "mylib/network.h"


#define MAX_BUFFER 1024


struct tcpserver {
	int server_fd_;
	std::map<int, sockaddr_t> sockaddrs_;
};


COBWEB_MOD_API tcpserver*
tcpserver_create(void) {
	NetworkSystem::init();
	tcpserver* inst = new tcpserver();
	inst->server_fd_ = NetworkSystem::tcp_create();
	return inst;
}

COBWEB_MOD_API void
tcpserver_release(tcpserver* inst) {
	NetworkSystem::socket_close(inst->server_fd_);
	NetworkSystem::release();
	delete inst;
}

static void
_recv(Context* ctx, tcpserver* inst) {
	auto iter = inst->sockaddrs_.begin();
	while (iter != inst->sockaddrs_.end()) {
		char buffer[MAX_BUFFER] = { 0 };
		int ret = NetworkSystem::tcp_recv(iter->second.sockfd, buffer);
		if (ret == 0) {
			/* close client */
			iter = inst->sockaddrs_.erase(iter);
			continue;
		}
		else if (ret > 0) {
			NetworkSystem::tcp_send(iter->second.sockfd, buffer, ret);
		}
		iter++;
	}
}

static void
_accept(Context* ctx, tcpserver* inst) {
	sockaddr_t sockaddr = NetworkSystem::tcp_accept(inst->server_fd_);
	if (sockaddr.sockfd != -1) {
		inst->sockaddrs_.insert(std::map<int, sockaddr_t>::value_type(sockaddr.sockfd, sockaddr));
	}
}

static bool
tcpserver_cb(Context* ctx, void* ud, int type, int session, uint32_t source, const void* data, size_t sz) {
	tcpserver* inst = (struct tcpserver*)ud;
	if (type == PTYPE_RESPONSE) {
		std::string msg((const char*)data);
		if (msg.compare("accept") == 0) {
			_accept(ctx, inst);
			std::string str("accept");
			ctx->timeout(ctx->get_handle(ctx), PTYPE_RESPONSE | PTYPE_TAG_STRING, 0, str.c_str(), str.size(), 10);
		}
		else if (msg.compare("recv") == 0) {
			_recv(ctx, inst);
			std::string str("recv");
			ctx->timeout(ctx->get_handle(ctx), PTYPE_RESPONSE | PTYPE_TAG_STRING, 0, str.c_str(), str.size(), 10);
		}
	}
	return true;
}

COBWEB_MOD_API bool
tcpserver_init(tcpserver* inst, Context* ctx, std::string param) {
	int index = param.find(':');
	if (index == -1) {
		return false;
	}
	std::string ip = param.substr(0, index);
	int port = std::stoi(param.substr(index + 1));
	if (!NetworkSystem::socket_bind(inst->server_fd_, ip.c_str(), port)) {
		return false;
	}
	if (!NetworkSystem::tcp_listen(inst->server_fd_)) {
		return false;
	}

	if (!NetworkSystem::socket_nonblock(inst->server_fd_)) {
		ctx->log_error(ctx->get_handle(ctx), "tcp set nonblock failed...");
		return false;
	}

	ctx->callback(ctx, inst, tcpserver_cb);
	ctx->command(ctx, "REG", ".tcpserver");
	std::string str1("accept");
	ctx->timeout(ctx->get_handle(ctx), PTYPE_RESPONSE | PTYPE_TAG_STRING, 0, str1.c_str(), str1.size(), 10);
	std::string str2("recv");
	ctx->timeout(ctx->get_handle(ctx), PTYPE_RESPONSE | PTYPE_TAG_STRING, 0, str2.c_str(), str2.size(), 10);
	return true;
}

COBWEB_MOD_API void
tcpserver_signal(tcpserver* inst, int signal) {

}

/***************************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_kcpserver.c
Description: kcpserver
Author: ydlc
Version: 1.0
Date: 2021.12.10
History:
****************************************************/


#include <stdint.h>
#include "cobweb.h"
#include "kcp/kcp.h"


#define DEFAULT_HEART 3


/* kcp client */
struct client_t {
	int fd; // fd
	ikcpcb* kcpcb;
	struct sockaddr_t saddr;
	int heart; // 心跳<=0时表示掉线
	uint64_t dead_time; // 时间戳
};

/* kcp server */
struct kcpserver_t {
	int serverfd; // server sockfd
	int id; // id
	struct linkedlist_t* clients; // clients
	size_t timeout; // 超时时间/s
	bool done;
};


static inline IINT64
_iclock64(void) {
	long s, u;
	IINT64 value;
	platform_timeofday(&s, &u);
	value = ((IINT64)s) * 1000 + (u / 1000);
	return value;
}

static inline IUINT32
_iclock() {
	return (IUINT32)(_iclock64() & 0xfffffffful);
}

static int
_output(const char* data, int size,
	ikcpcb* kcpcb, void* user) {
	int ret = 0;
	struct client_t* client = (struct client_t*)user;
	if (client != NULL) {
		ret = platform_udp_send(client->saddr.sockfd, data, size, client->saddr.ip, client->saddr.port);
	}
	return ret;
}

struct client_t*
_kcpclient_new(int fd, int serverfd, const char* ip, int port) {
	struct client_t* client = (struct client_t*)cobweb_malloc(sizeof(struct client_t));
	if (client != NULL) {
		client->fd = fd;
		client->kcpcb = ikcp_create(client->fd, client);
		client->saddr.sockfd = serverfd;
		strcpy(client->saddr.ip, ip);
		client->saddr.port = port;
		client->heart = DEFAULT_HEART;
		client->dead_time = cobweb_timestamp();
		ikcp_wndsize(client->kcpcb, 1024, 1024);
		ikcp_setoutput(client->kcpcb, _output);
	}

	return client;
}

static bool
_update(void* data, void* arg) {
	struct client_t* client = (struct client_t*)data;
	struct kcpserver_t* inst = (struct kcpserver_t*)arg;
	if (client->heart > 0) {
		uint64_t curr_time = cobweb_timestamp();
		uint64_t ms = inst->timeout * 1000; // 换算成秒
		if (client->dead_time + ms <= curr_time) {
			client->dead_time = curr_time;
			client->heart--;
		}

		ikcp_update(client->kcpcb, ikcp_check(client->kcpcb, _iclock()));
	}

	return 0;
}

static bool
_query_client(void* data, void* arg) {
	struct client_t* client = (struct client_t*)data;
	int fd = (int)arg;
	return client->fd == fd ? true : false;
}

static bool
_clear(void* data, void* arg) {
	struct client_t* client = (struct client_t*)data;
	if (client->heart <= 0) {
		struct socket_message_t* socket_message = (struct socket_message_t*)cobweb_malloc(sizeof(struct socket_message_t));
		if (socket_message != NULL) {
			socket_message->fd = client->fd;
			socket_message->type = SOCKET_TYPE_CLOSE;
			socket_message->sz = 0;
			memset(socket_message->buffer, 0, COBWEB_MESSAGE_SIZE);
			cobweb_context_ssend(socket_message);
		}
		ikcp_release(client->kcpcb);
		return true;
	}
	else {
		return false;
	}
}

// 对应c#   = BitConverter.ToUInt32
static void
_write_int(uint8_t* buffer, int value) {
	buffer[3] = ((0xff000000 & value) >> 24);
	buffer[2] = ((0xff0000 & value) >> 16);
	buffer[1] = ((0xff00 & value) >> 8);
	buffer[0] = (0xff & value);
}

// 对应c# = BitConverter.ToUInt32
static int
_read_int(uint8_t* buffer) {
	int r = buffer[0];
	r |= (int)buffer[1] << 8;
	r |= (int)buffer[2] << 16;
	r |= (int)buffer[3] << 24;
	return r;
}

static void
_accept(struct kcpserver_t* inst, const char* ip, int port) {
	inst->id++;
	struct client_t* client = _kcpclient_new(inst->id, inst->serverfd, ip, port);
	if (client == NULL) {
		platform_log("create client failed!");
		return;
	}

	bool ret = cobweb_linkedlist_insert(inst->clients, client);
	if (ret) {
		uint8_t buffer[4] = { 0 };
		_write_int(buffer, client->fd);
		platform_udp_send(client->saddr.sockfd, (char*)buffer, 4, ip, port);
		struct socket_message_t* socket_message = (struct socket_message_t*)cobweb_malloc(sizeof(struct socket_message_t));
		if (socket_message != NULL) {
			socket_message->fd = client->fd;
			socket_message->type = SOCKET_TYPE_ACCEPT;
			socket_message->sz = 0;
			memset(socket_message->buffer, 0, COBWEB_MESSAGE_SIZE);
			cobweb_context_ssend(socket_message);
		}
	}
}

static void
_recv(struct kcpserver_t* inst, 
	int conv, const char* buffer, int size) {
	struct client_t* client = (struct client_t*)cobweb_linkedlist_query(inst->clients, _query_client, (void*)conv);
	if (client == NULL) {
		return;
	}
	ikcp_input(client->kcpcb, buffer, size);
	while (true) {
		int peeksize = ikcp_peeksize(client->kcpcb);
		if (peeksize <= 0) {
			break;
		}
		if (peeksize > COBWEB_MESSAGE_SIZE) {
			break;
		}
		client->dead_time = cobweb_timestamp();
		char* buf = (char*)cobweb_malloc(sizeof(char) * peeksize);
		if (ikcp_recv(client->kcpcb, buf, peeksize) > 0) {
			client->heart = DEFAULT_HEART;
			struct socket_message_t* socket_message = (struct socket_message_t*)cobweb_malloc(sizeof(struct socket_message_t));
			if (socket_message != NULL) {
				socket_message->fd = client->fd;
				socket_message->type = SOCKET_TYPE_DATA;
				socket_message->sz = peeksize;
				memcpy(socket_message->buffer, buf, peeksize);
				socket_message->buffer[socket_message->sz] = 0;
				cobweb_context_ssend(socket_message);
			}
			cobweb_free (buf);
		}
	}
}

struct kcpserver_t*
cobweb_kcpserver_create(const char* ip, int port) {
	struct kcpserver_t* inst = (struct kcpserver_t*)cobweb_malloc(sizeof(struct kcpserver_t));
	assert(inst != NULL);
	memset(inst, 0, sizeof(struct kcpserver_t));
	inst->id = 300;
	inst->clients = cobweb_linkedlist_create();
	inst->serverfd = platform_udp_create();
	inst->done = true;
	inst->timeout = 120;
	platform_socket_nonblock(inst->serverfd);
	if (!platform_socket_bind(inst->serverfd, ip, port)) {
		platform_red_print("udp bind error!");
	}
	return inst;
}


int
cobweb_kcpserver_send(struct kcpserver_t* inst, int conv, const char* buffer, size_t sz) {
	struct client_t* client = (struct client_t*)cobweb_linkedlist_query(inst->clients, _query_client, (void*)conv);
	if (client != NULL) {
		return ikcp_send(client->kcpcb, buffer, (int)sz);
	}
	else {
		return 0;
	}
}

void
cobweb_kcpserver_dispatch(struct kcpserver_t* inst) {
	while (inst->done) {
		cobweb_linkedlist_each(inst->clients, _update, inst);
		char buffer[COBWEB_MESSAGE_SIZE] = { 0 };
		char ip[COBWEB_MAX_IP] = { 0 };
		int port = 0;
		int ret = platform_udp_recv(inst->serverfd, buffer, ip, &port);
		if (ret != -1 && ret >= 2) {
			int conv = _read_int((uint8_t*)buffer);
			if (conv == 0) {
				_accept(inst, ip, port);
			}
			else {
				_recv(inst, conv, buffer, ret);
			}
		}

		cobweb_linkedlist_drainf(inst->clients, _clear, inst);
	}
}

void 
cobweb_kcpserver_loopexit(struct kcpserver_t* inst) {
	inst->done = false;
}

void
cobweb_kcpserver_release(struct kcpserver_t* inst) {
	if (inst != NULL) {
		platform_socket_close(inst->serverfd);
		cobweb_linkedlist_release(inst->clients);
		cobweb_free (inst);
		inst = NULL;
	}
}


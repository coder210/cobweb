/*
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_tcpserver.c
Description: 由libevent实现的tcp server
Author: ydlc
Version: 1.0
Date: 2021.12.10
History:
**/

#include <ctype.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/thread.h>
#include "cobweb.h"


#ifdef _WIN32
#pragma comment(lib, "libevent.lib")
#pragma comment(lib, "libevent_core.lib")
#pragma comment(lib, "libevent_extras.lib")
#elif _LINUX
// inet_ntoa x64需要使用这个头文件,不然会报错
#include <arpa/inet.h>
#else
#endif


static void
_evthread_use_pthreads() {
#ifdef _WIN32
	evthread_use_windows_threads();
#elif _LINUX
	evthread_use_pthreads();
#else
#endif
}

#define DEFAULT_PARSE 0
#define TEXT_PARSE 1
#define BINARY_PARSE 2

#define SEP_BEGIN 0x02
#define SEP_END 0x03


/* client */
struct client_t {
	int fd;
	char ip[COBWEB_MAX_IP];
	int port;
	struct bufferevent* bufev;
};

/* libevent server */
struct tcpserver_t {
	struct evconnlistener* evconn;
	int serverfd; // server sockfd
	int proto_parse; // 协议解析方式, 0: 不做任何处理, 1:字符处理, 2: 字节处理
	struct linkedlist_t* clients; // clients
	size_t timeout; // 超时时间/s
};


short
_read_short(uint8_t* buffer) {
	short r = (short)buffer[0] << 8 | (short)buffer[1];
	return r;
}

void
_write_short(uint8_t* buffer, short value) {
	buffer[0] = (value >> 8) & 0xff;
	buffer[1] = value & 0xff;
}

static void
_write_cb(struct bufferevent* bev, void* arg) {
	/*struct tcpserver_t* inst = (struct tcpserver_t*)arg;
	evutil_socket_t clientfd = bufferevent_getfd(bev);*/
}


// 二进制解析
static void
_binary_parse(struct bufferevent* buffev,
	struct tcpserver_t* inst) {
	evutil_socket_t clientfd = bufferevent_getfd(buffev);
	struct evbuffer* input_evbuffer = bufferevent_get_input(buffev);
	char buf[COBWEB_MESSAGE_SIZE] = { 0 };
	while (evbuffer_get_length(input_evbuffer) > sizeof(short)) {
		evbuffer_copyout(input_evbuffer, buf, sizeof(short));
		size_t sz = _read_short((uint8_t*)buf);
		if (sz < COBWEB_MESSAGE_SIZE) {
			if (evbuffer_get_length(input_evbuffer) >= sz) {
				evbuffer_drain(input_evbuffer, sizeof(short));
				int ret = evbuffer_remove(input_evbuffer, buf, sz);
				if (ret != -1) {
					struct socket_message_t* socket_message = (struct socket_message_t*)cobweb_malloc(sizeof(struct socket_message_t));
					if (socket_message != NULL) {
						socket_message->fd = clientfd;
						socket_message->type = SOCKET_TYPE_DATA;
						socket_message->sz = sz;
						memcpy(socket_message->buffer, buf, sz);
						socket_message->buffer[socket_message->sz] = 0;
						cobweb_context_ssend(socket_message);
					}
				}
			}
			else {
				bufferevent_disable(buffev, EV_READ);
				bufferevent_setwatermark(buffev, EV_READ, sz, 0);
				bufferevent_enable(buffev, EV_READ);
				break;
			}
		}
		else {
			// 包太长,可能是恶意攻击
			// 直接丢掉
			bufferevent_free(buffev);
			break;
		}
	}
}

// 文本解析
static void
_text_parse(struct bufferevent* bufev, struct tcpserver_t* inst) {
	evutil_socket_t clientfd = bufferevent_getfd(bufev);
	int ret;
	char beginbuff = toascii(SEP_BEGIN);
	char endbuff = toascii(SEP_END);
	struct evbuffer* input_evbuffer = bufferevent_get_input(bufev);
	struct evbuffer_ptr begin, end;
	while (true) {
		/*读取0x02开头，0x03结尾的数据包*/
		begin = evbuffer_search(input_evbuffer, &beginbuff, 1, NULL);
		if (begin.pos >= 0) {
			end = evbuffer_search(input_evbuffer, &endbuff, 1, &begin);
			if (end.pos >= begin.pos) {
				// 拷贝真正数据
				size_t data_sz = end.pos - begin.pos - 1;
				if (data_sz == 0) {
					// 为0说明两者之间没有数据
					// 删除分隔符即可
					evbuffer_drain(input_evbuffer, end.pos + 1);
				}
				else if (data_sz > COBWEB_MESSAGE_SIZE) {
					// 包太长,可能是恶意攻击
					// 直接丢掉
					bufferevent_free(bufev);
					break;
				}
				else {
					// 有数据
					// 删除头分隔符即可
					// 先截出数据
					// 再删除尾分隔符即可
					evbuffer_drain(input_evbuffer, begin.pos + 1);
					char buf[COBWEB_MESSAGE_SIZE] = { 0 };
					ret = evbuffer_remove(input_evbuffer, buf, data_sz);
					evbuffer_drain(input_evbuffer, data_sz + 1);
					if (ret != -1) {
						struct socket_message_t* socket_message = (struct socket_message_t*)cobweb_malloc(sizeof(struct socket_message_t));
						if (socket_message != NULL) {
							socket_message->fd = clientfd;
							socket_message->type = SOCKET_TYPE_DATA;
							socket_message->sz = data_sz;
							memcpy(socket_message->buffer, buf, data_sz);
							socket_message->buffer[socket_message->sz] = 0;
							cobweb_context_ssend(socket_message);
						}
					}
				}
			}
			else {
				break;
			}
		}
		else {
			break;
		}
	}
}

// 不做任何处理
static void
_default_parse(struct bufferevent* bufev, struct tcpserver_t* inst) {
	evutil_socket_t clientfd = bufferevent_getfd(bufev);
	int ret;
	struct evbuffer* input_evbuffer = bufferevent_get_input(bufev);
	size_t sz = evbuffer_get_length(input_evbuffer);
	if (sz > COBWEB_MESSAGE_SIZE) {
		// 包太长,可能是恶意攻击
		// 直接丢掉
		bufferevent_free(bufev);
	}
	else {
		char buf[COBWEB_MESSAGE_SIZE] = { 0 };
		ret = evbuffer_remove(input_evbuffer, buf, COBWEB_MESSAGE_SIZE - 1);
		struct socket_message_t* socket_message = (struct socket_message_t*)cobweb_malloc(sizeof(struct socket_message_t));
		if (socket_message != NULL) {
			socket_message->fd = clientfd;
			socket_message->type = SOCKET_TYPE_DATA;
			socket_message->sz = ret;
			memcpy(socket_message->buffer, buf, ret);
			socket_message->buffer[socket_message->sz] = 0;
			cobweb_context_ssend(socket_message);
		}
	}

}

// 数据接收
static void
_read_cb(struct bufferevent* bufev, void* arg) {
	struct tcpserver_t* inst = (struct tcpserver_t*)arg;
	switch (inst->proto_parse) {
	case DEFAULT_PARSE:
		_default_parse(bufev, inst);
		break;
	case TEXT_PARSE:
		_text_parse(bufev, inst);
		break;
	case BINARY_PARSE:
		_binary_parse(bufev, inst);
		break;
	default:
		_default_parse(bufev, inst);
		break;
	}
}

bool
_drain_client(void* data, void* arg) {
	struct client_t* client = (struct client_t*)data;
	int fd = (int)arg;
	return client->fd == fd;
}

static void
_event_cb(struct bufferevent* bufev,
	short what, void* arg) {
	struct tcpserver_t* inst = (struct tcpserver_t*)arg;
	int clientfd = bufferevent_getfd(bufev);
	// error
	if (BEV_EVENT_ERROR & what) {
		goto OFFLINE;
	}

	// ex
	if (BEV_EVENT_EOF & what) {
		goto OFFLINE;
	}

	// timeout
	if (BEV_EVENT_TIMEOUT & what) {
		goto OFFLINE;
	}

	return;
OFFLINE:
	if (cobweb_linkedlist_drainf(inst->clients, _drain_client, (void*)clientfd)) {
		bufferevent_free(bufev);
		struct socket_message_t* socket_message = (struct socket_message_t*)cobweb_malloc(sizeof(struct socket_message_t));
		if (socket_message != NULL) {
			socket_message->fd = clientfd;
			socket_message->type = SOCKET_TYPE_CLOSE;
			socket_message->sz = 0;
			socket_message->buffer[socket_message->sz] = 0;
			cobweb_context_ssend(socket_message);
		}
	}
}

static void
_accept_cb(struct evconnlistener* listener,
	evutil_socket_t clientfd, struct sockaddr* addr,
	int len, void* arg) {
	struct tcpserver_t* inst = (struct tcpserver_t*)arg;
	struct event_base* base = evconnlistener_get_base(listener);
	evutil_make_socket_nonblocking(clientfd);
	struct bufferevent* bev = bufferevent_socket_new(base, clientfd, BEV_OPT_THREADSAFE | BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
	bufferevent_setcb(bev, (bufferevent_data_cb)_read_cb
		, (bufferevent_data_cb)_write_cb, (bufferevent_event_cb)_event_cb, arg);

	struct timeval timeout_read = { inst->timeout, 0 };
	bufferevent_set_timeouts(bev, &timeout_read, NULL);
	bufferevent_setwatermark(bev, EV_READ, 2, 0);
	bufferevent_setwatermark(bev, EV_WRITE, 2, 0);
	bufferevent_enable(bev, EV_READ | EV_WRITE);

	struct sockaddr_in* addr_in = (struct sockaddr_in*)addr;
	const char* ip = inet_ntoa(addr_in->sin_addr);
	int port = ntohs(addr_in->sin_port);
	struct client_t* client = (struct client_t*)cobweb_malloc(sizeof(struct client_t));
	if (client != NULL) {
		client->fd = clientfd;
		client->bufev = bev;
		sprintf(client->ip, ip);
		client->port = port;
	}

	if (cobweb_linkedlist_insert(inst->clients, client)) {
		struct socket_message_t* socket_message = (struct socket_message_t*)cobweb_malloc(sizeof(struct socket_message_t));
		if (socket_message != NULL) {
			socket_message->fd = clientfd;
			socket_message->type = SOCKET_TYPE_ACCEPT;
			socket_message->sz = 0;
			socket_message->buffer[socket_message->sz] = 0;
			cobweb_context_ssend(socket_message);
		}
	}
}

struct tcpserver_t*
cobweb_tcpserver_create(const char* ip, int port, int proto_parse) {
	struct tcpserver_t* inst = (struct tcpserver_t*)cobweb_malloc(sizeof(struct tcpserver_t));
	assert(inst != NULL);
	memset(inst, 0, sizeof(struct tcpserver_t));
	inst->clients = cobweb_linkedlist_create();
	_evthread_use_pthreads();
	inst->proto_parse = proto_parse;
	struct event_base* base = event_base_new();
	struct sockaddr_in serveraddr = ccp_sockaddr(ip, port);
	struct evconnlistener* evconn = evconnlistener_new_bind(base,
		_accept_cb,
		inst,
		LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,
		512,
		(struct sockaddr*)&serveraddr,
		sizeof(struct sockaddr_in));
	if (evconn != NULL) {
		inst->evconn = evconn;
		inst->serverfd = evconnlistener_get_fd(evconn);
	}
	return inst;
}

void
cobweb_tcpserver_dispatch(struct tcpserver_t* inst) {
	struct event_base* base = evconnlistener_get_base(inst->evconn);
	event_base_dispatch(base);
}

void 
cobweb_tcpserver_loopexit(struct tcpserver_t* inst) {
	struct event_base* base = evconnlistener_get_base(inst->evconn);
	event_base_loopexit(base, 0);
}

void
cobweb_tcpserver_release(struct tcpserver_t* inst) {
	if (inst != NULL) {
		struct event_base* base = evconnlistener_get_base(inst->evconn);
		evconnlistener_free(inst->evconn);
		event_base_free(base);
		cobweb_linkedlist_release(inst->clients);
		cobweb_free (inst);
		inst = NULL;
	}
}

static bool
_query_client(void* data, void* arg) {
	struct client_t* client = (struct client_t*)data;
	int fd = (int)arg;
	return client->fd == fd ? true : false;
}

int
cobweb_tcpserver_send(struct tcpserver_t* inst, int fd, const char* buffer, size_t sz) {
	int ret = 0;
	struct client_t* client = (struct client_t*)cobweb_linkedlist_query(inst->clients, _query_client, (void*)fd);
	if (client == NULL) {
		return ret;
	}
	if (inst->proto_parse == DEFAULT_PARSE) {
		ret = bufferevent_write(client->bufev, buffer, sz);
	}
	else if (inst->proto_parse == TEXT_PARSE) {
		char* send_buffer = (char*)cobweb_malloc(sizeof(char) * (sz + 2));
		if (send_buffer != NULL) {
			send_buffer[0] = SEP_BEGIN;
			memcpy(send_buffer + 1, buffer, sz);
			send_buffer[sz + 1] = SEP_END;
			ret = bufferevent_write(client->bufev, send_buffer, sz + 2);
			cobweb_free (send_buffer);
		}
	}
	else if (inst->proto_parse == BINARY_PARSE) {
		char* send_buffer = (char*)cobweb_malloc(sizeof(char) * (sz + 2));
		if (send_buffer != NULL) {
			_write_short((uint8_t*)send_buffer, (short)sz);
			memcpy(send_buffer + 2, buffer, sz);
			ret = bufferevent_write(client->bufev, send_buffer, sz + 2);
			cobweb_free (send_buffer);
		}
	}
	return ret;
}



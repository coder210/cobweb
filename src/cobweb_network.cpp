/*************************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_network.c
Description: 网络相关实现, libevent, kcp等
Author: ydlc
Version: 1.0
Date: 2021.12.10
History:
**************************************************/
#include "cobweb.h"


#define AE_KCP 1
#define AE_TCP 2


/* socket server */
struct socket_server_t {
	struct kcpserver_t* kcpserver;
	struct tcpserver_t* tcpserver;
	int afterend;
};


static struct socket_server_t* S = NULL;


/*
* address: 地址
* afterend: 后端方式kcp=1 tcp=2
* proto_parse: 协议解析方式default=0 text=1 binrary=2
*/
void
cobweb_sserver_init(char* address, int afterend, int proto_parse){
	S = (struct socket_server_t*)cobweb_malloc(sizeof(struct socket_server_t));
	assert(S != NULL);
	S->afterend = afterend;
	const char* ip = cobweb_strsep(&address, ":");
	int port = atoi(cobweb_strsep(&address, ":"));
	if (S->afterend == AE_KCP) {
		S->kcpserver = cobweb_kcpserver_create(ip, port);
	}
	else if (S->afterend == AE_TCP) {
		S->tcpserver = cobweb_tcpserver_create(ip, port, proto_parse);
	}
}

int
cobweb_sserver_dispatch(void) {
	if (S != NULL) {
		if (S->afterend == AE_KCP) {
			cobweb_kcpserver_dispatch(S->kcpserver);
		}
		else if (S->afterend == AE_TCP) {
			cobweb_tcpserver_dispatch(S->tcpserver);
		}
	}
	return 1;
}

void 
cobweb_sserver_loopexit(void) {
	if (S != NULL) {
		if (S->afterend == AE_KCP) {
			cobweb_kcpserver_loopexit(S->kcpserver);
		}
		else if (S->afterend == AE_TCP) {
			cobweb_tcpserver_loopexit(S->tcpserver);
		}
	}
}

int
cobweb_sserver_send(int fd, const char* buffer, size_t sz) {
	int ret = 0;
	if (S != NULL) {
		if (S->afterend == AE_KCP) {
			ret = cobweb_kcpserver_send(S->kcpserver, fd, buffer, sz);
		}
		else if (S->afterend == AE_TCP) {
			ret = cobweb_tcpserver_send(S->tcpserver, fd, buffer, sz);
		}
	}
	return ret;
}

void
cobweb_sserver_release(void) {
	if (S != NULL) {
		if (S->afterend == AE_KCP) {
			cobweb_kcpserver_release(S->kcpserver);
		}
		else if (S->afterend == AE_TCP) {
			cobweb_tcpserver_release(S->tcpserver);
		}
		cobweb_free (S);
		S = NULL;
	}
}



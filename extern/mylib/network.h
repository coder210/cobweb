#ifndef _NETWORK_H
#define _NETWORK_H

struct sockaddr_t {
	int sockfd;
	char ip[128];
	int port;
};

class NetworkSystem {
public:
	static bool init(void);
	static bool release(void);
	static int anyaddr(void);
	static bool socket_close(int);
	static bool socket_nonblock(int);
	static bool socket_bind(int, const char*, int);
	static int tcp_create(void);
	static bool tcp_connect(int, const char*, int);
	static bool tcp_listen(int);
	static sockaddr_t tcp_accept(int);
	static int tcp_send(int, const char*, int);
	static int tcp_recv(int, char*);
	static int tcp_noblock_recv(int, char*);
	static int udp_create(void);
	static int udp_recv(int, char*, char*, int*);
	static int udp_send(int, const char*, int, const char*, int);
};



#endif

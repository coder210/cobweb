/*************************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: network.cpp
Description: 网络接口
Author: ydlc
Version: 1.0
Date: 2021.12.10
History:
**************************************************/
#include "network.h"
#include <string.h>

#define MAX_BUFFER 1024
#define MAX_IP 127 

#ifdef _COBWEB_WIN32
#include <ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

#elif _COBWEB_LINUX
#include<unistd.h>
#include<errno.h>
#include<sys/ioctl.h>
#include<sys/fcntl.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#endif


sockaddr_in
_create_sockaddr(const char* ip, int port) {
	sockaddr_in peer;
	peer.sin_family = AF_INET;
	peer.sin_addr.s_addr = inet_addr(ip);
	peer.sin_port = htons(port);
	return peer;
}

bool
NetworkSystem::init(void) {
#ifdef _COBWEB_WIN32
	WSADATA wsa_data;
	int ret = WSAStartup(0x0201, &wsa_data);
	return ret != 0 ? true : false;
#elif _COBWEB_LINUX
	return true;
#endif
}

bool
NetworkSystem::release(void) {
#ifdef _COBWEB_WIN32
	int ret = WSACleanup();
	return ret != 0 ? true : false;
#elif _COBWEB_LINUX
	return true;
#endif
}

int
NetworkSystem::anyaddr(void) {
#ifdef _COBWEB_WIN32
	int anyaddr = htonl(INADDR_ANY);
	return anyaddr;
#elif _COBWEB_LINUX
	int anyaddr = (int)htonl(INADDR_ANY);
	return anyaddr;
#endif
}

bool
NetworkSystem::socket_close(int sockfd) {
#ifdef _COBWEB_WIN32
	return closesocket(sockfd) == 0 ? true : false;
#elif _COBWEB_LINUX
	return close(sockfd) == 0 ? true : false;
#endif
}


bool
NetworkSystem::socket_nonblock(int sockfd) {
#ifdef _COBWEB_WIN32
	unsigned long ul = 1;
	int ret = ioctlsocket(sockfd, FIONBIO, (unsigned long*)&ul);
	return ret != SOCKET_ERROR ? true : false;
#elif _COBWEB_LINUX
	//int old_flag = fcntl(sockfd, F_GETFL);
	//int new_flag = old_flag | O_NONBLOCK;
	//int ret = fcntl(sockfd, F_SETFL, new_flag);
	//return ret != -1 ? true : false;
	int ret = ioctl(sockfd, (int)FIONBIO);
	return ret == 0 ? true : false;
#endif
}

bool
NetworkSystem::socket_bind(int sockfd, const char* ip, int port) {
#ifdef _COBWEB_WIN32
	sockaddr_in peer = _create_sockaddr(ip, port);
	int ret = bind(sockfd, (sockaddr*)&peer, sizeof(peer));
	return ret == 0 ? true : false;
#elif _COBWEB_LINUX
	sockaddr_in peer = _create_sockaddr(ip, port);
	int ret = bind(sockfd, (sockaddr*)&peer, sizeof(peer));
	return ret == 0 ? true : false;
#endif
}

bool
NetworkSystem::tcp_connect(int sockfd, const char* ip, int port) {
#ifdef _COBWEB_WIN32
	sockaddr_in peer = _create_sockaddr(ip, port);
	if (connect(sockfd, (sockaddr*)&peer, sizeof(peer)) == 0) {
		return true;
	}
	else {
		return false;
	}
#elif _COBWEB_LINUX
	sockaddr_in peer = _create_sockaddr(ip, port);
	if (connect(sockfd, (sockaddr*)&peer, sizeof(peer)) == 0) {
		return true;
	}
	else {
		return false;
	}
#endif
}

int
NetworkSystem::tcp_create(void) {
#ifdef _COBWEB_WIN32
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	return sockfd;
#elif _COBWEB_LINUX
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	return sockfd;
#endif
}

bool
NetworkSystem::tcp_listen(int sockfd) {
#ifdef _COBWEB_WIN32
	return listen(sockfd, SOMAXCONN) == 0 ? true : false;
#elif _COBWEB_LINUX
	return listen(sockfd, SOMAXCONN) == 0 ? true : false;
#endif
}

sockaddr_t
NetworkSystem::tcp_accept(int sockfd) {
	sockaddr_t sockaddr;
	memset(&sockaddr, 0, sizeof(struct sockaddr_t));
#ifdef _COBWEB_WIN32
	sockaddr_in client;
	int len = sizeof(client);
	int clientfd = accept(sockfd, (struct sockaddr*)&client, &len);
	sockaddr.sockfd = clientfd;
	strncpy_s(sockaddr.ip, inet_ntoa(client.sin_addr), MAX_IP);
	sockaddr.port = ntohs(client.sin_port);
#elif _COBWEB_LINUX
	sockaddr_in client;
	socklen_t len = sizeof(client);
	int clientfd = accept(sockfd, (struct sockaddr*)&client, &len);
	sockaddr.sockfd = clientfd;
	strncpy(sockaddr.ip, inet_ntoa(client.sin_addr), MAX_IP);
	sockaddr.port = ntohs(client.sin_port);
#endif
	return sockaddr;
}

int
NetworkSystem::tcp_send(int sockfd, const char* buf, int len) {
#ifdef _COBWEB_WIN32
	int n = send(sockfd, buf, len, 0);
	return n;
#elif _COBWEB_LINUX
	int n = send(sockfd, buf, len, 0);
	return n;
#endif
}

int
NetworkSystem::tcp_recv(int sockfd, char* buffer) {
#ifdef _COBWEB_WIN32
	char buf[MAX_BUFFER] = { 0 };
	memset(buf, 0, MAX_BUFFER);
	int size = recv(sockfd, buf, MAX_BUFFER, 0);
	if (size > 0) {
		memcpy(buffer, buf, size);
	}
	return size;
#elif _COBWEB_LINUX
	char buf[MAX_BUFFER] = { 0 };
	memset(buf, 0, MAX_BUFFER);
	int size = recv(sockfd, buf, MAX_BUFFER, 0);
	if (size > 0) {
		memcpy(buffer, buf, size);
	}
	return size;
#endif
}

int
NetworkSystem::tcp_noblock_recv(int sockfd, char* buffer) {
#ifdef _COBWEB_WIN32
	char buf[MAX_BUFFER] = { 0 };
	memset(buf, 0, MAX_BUFFER);
	int size = recv(sockfd, buf, MAX_BUFFER, 0);
	if (size > 0) {
		memcpy(buffer, buf, size);
	}
	return size;
#elif _COBWEB_LINUX
	char buf[MAX_BUFFER] = { 0 };
	memset(buf, 0, MAX_BUFFER);
	int size = recv(sockfd, buf, MAX_BUFFER, MSG_DONTWAIT);
	if (size > 0) {
		memcpy(buffer, buf, size);
	}
	return size;
#endif
}

int
NetworkSystem::udp_create(void) {
#ifdef _COBWEB_WIN32
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	return sockfd;
#elif _COBWEB_LINUX
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	return sockfd;
#endif
}

int
NetworkSystem::udp_recv(int sockfd, char* out_buffer, char* ip, int* port) {
#ifdef _COBWEB_WIN32
	sockaddr_in clientaddr;
	int len = sizeof(clientaddr);
	char buffer[MAX_BUFFER] = { 0 };
	int size = recvfrom(sockfd, buffer, MAX_BUFFER, 0, (sockaddr*)&clientaddr, &len);
	if (size > 0) {
		memcpy(out_buffer, buffer, size);
	}
	strcpy(ip, inet_ntoa(clientaddr.sin_addr));
	*port = ntohs(clientaddr.sin_port);
	return size;
#elif _COBWEB_LINUX
	sockaddr_in clientaddr;
	int len = sizeof(clientaddr);
	char buffer[MAX_BUFFER] = { 0 };
	int size = recvfrom(sockfd, buffer, MAX_BUFFER, 0, (sockaddr*)&clientaddr, (socklen_t*)&len);
	if (size > 0) {
		memcpy(out_buffer, buffer, size);
	}
	strcpy(ip, inet_ntoa(clientaddr.sin_addr));
	*port = ntohs(clientaddr.sin_port);
	return size;
#endif
}

int
NetworkSystem::udp_send(int sockfd, const char* buffer, int len,
	const char* ip, int port) {
#ifdef _COBWEB_WIN32
	sockaddr_in addr = _create_sockaddr(ip, port);
	int n = sendto(sockfd, buffer, len, 0, (sockaddr*)&addr, sizeof(addr));
	return n;
#elif _COBWEB_LINUX
	sockaddr_in addr = _create_sockaddr(ip, port);
	int n = sendto(sockfd, buffer, len, 0, (struct sockaddr*)&addr, sizeof(addr));
	return n;
#endif
}

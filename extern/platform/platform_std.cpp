/*************************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: platform_std.c
Description: 跨平台接口
Author: ydlc
Version: 1.0
Date: 2021.12.10
History:
**************************************************/
#include "platform.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

/* 一个包建议长度 */
#define CC_MESSAGE_SIZE 2048
#define CC_MAX_BUFFER 1024
#define CC_MAX_PATH 255
#define CC_MAX_IP 128

#ifdef _COBWEB_WIN32
#include <ws2tcpip.h>
#include <Windows.h>
#include <process.h>
#pragma comment(lib,"ws2_32.lib")

enum FG_COLOR_E
{
	FG_NONE = 0x1111,
	FG_BLACK = 0x0,
	FG_RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
	FG_GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	FG_YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
	FG_BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	FG_PURPLE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
	FG_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
};

enum BG_COLOR_E
{
	BG_NONE = 0x1111,
	BG_BLACK = 0x0,
	BG_RED = BACKGROUND_INTENSITY | BACKGROUND_RED,
	BG_GREEN = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
	BG_BLUE = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
	BG_YELLOW = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
	BG_PURPLE = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
	BG_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
};
#elif _COBWEB_LINUX
#include<unistd.h>
#include<errno.h>
#include<dlfcn.h>
#include<pthread.h>
#include<sys/fcntl.h>
#include<sys/time.h>
#include<sys/socket.h>
#include<arpa/inet.h>

// color
enum FG_COLOR_E {
	FG_NONE = 0,
	FG_BLACK = 30,
	FG_RED = 31,
	FG_GREEN = 32,
	FG_YELLOW = 33,
	FG_BLUE = 34,
	FG_PURPLE = 35,
	FG_WHITE = 37
};

enum BG_COLOR_E {
	BG_NONE = 0,
	BG_BLACK = 40,
	BG_RED = 41,
	BG_GREEN = 42,
	BG_YELLOW = 43,
	BG_BLUE = 44,
	BG_PURPLE = 45,
	BG_WHITE = 47
};

#endif


/* mutex */
struct mutex_t {
#ifdef _COBWEB_WIN32 /* win32 env */
	CRITICAL_SECTION m;
#elif _COBWEB_LINUX /* linux env */
	pthread_mutex_t m;
#endif /* undefined env */
};

/* event */
struct event_t {
	char name[CC_MAX_PATH];
	struct mutex_t* mutex;
#ifdef _COBWEB_WIN32 /* win32 env */
	HANDLE handle;
#elif _COBWEB_LINUX /* linux env */
	pthread_cond_t handle;
#endif /* undefined env */
};


bool
platform_network_init(void) {
#ifdef _COBWEB_WIN32
	WSADATA wsa_data;
	int ret = WSAStartup(0x0201, &wsa_data);
	return ret != 0 ? true : false;
#elif _COBWEB_LINUX
	return true;
#endif
}

bool
platform_network_release(void) {
#ifdef _COBWEB_WIN32
	int ret = WSACleanup();
	return ret != 0 ? true : false;
#elif _COBWEB_LINUX
	return true;
#endif
}

int
platform_anyaddr(void) {
#ifdef _COBWEB_WIN32
	int anyaddr = htonl(INADDR_ANY);
	return anyaddr;
#elif _COBWEB_LINUX
	int anyaddr = (int)htonl(INADDR_ANY);
	return anyaddr;
#endif
}

bool
platform_socket_close(int sockfd) {
#ifdef _COBWEB_WIN32
	return closesocket(sockfd) == 0 ? true : false;
#elif _COBWEB_LINUX
	return close(sockfd) == 0 ? true : false;
#endif
}

struct sockaddr_in
	platform_sockaddr(const char* ip, int port) {
#ifdef _COBWEB_WIN32
	struct sockaddr_in peer;
	peer.sin_family = AF_INET;
	peer.sin_addr.s_addr = inet_addr(ip);
	peer.sin_port = htons(port);
	return peer;
#elif _COBWEB_LINUX
	struct sockaddr_in peer;
	peer.sin_family = AF_INET;
	peer.sin_addr.s_addr = inet_addr(ip);
	peer.sin_port = htons(port);
	return peer;
#endif
}

bool
platform_socket_nonblock(int sockfd) {
#ifdef _COBWEB_WIN32
	unsigned long ul = 1;
	int ret = ioctlsocket(sockfd, FIONBIO, (unsigned long*)&ul);
	return ret != SOCKET_ERROR ? true : false;
#elif _COBWEB_LINUX
	int flag = fcntl(sockfd, F_GETFL);
	flag |= O_NONBLOCK;
	int ret = fcntl(sockfd, F_SETFL, flag);
	return ret != -1 ? true : false;
#endif
}

bool
platform_socket_bind(int sockfd, const char* ip, int port) {
#ifdef _COBWEB_WIN32
	struct sockaddr_in peer = platform_sockaddr(ip, port);
	int ret = bind(sockfd, (struct sockaddr*)&peer, sizeof(peer));
	return ret == 0 ? true : false;
#elif _COBWEB_LINUX
	struct sockaddr_in peer = platform_sockaddr(ip, port);
	int ret = bind(sockfd, (struct sockaddr*)&peer, sizeof(peer));
	return ret == 0 ? true : false;
#endif
}

bool
platform_tcp_connect(int sockfd, const char* ip, int port) {
#ifdef _COBWEB_WIN32
	struct sockaddr_in peer = platform_sockaddr(ip, port);
	if (connect(sockfd, (struct sockaddr*)&peer, sizeof(peer)) == 0) {
		return true;
	}
	else {
		return false;
	}
#elif _COBWEB_LINUX
	struct sockaddr_in peer = platform_sockaddr(ip, port);
	if (connect(sockfd, (struct sockaddr*)&peer, sizeof(peer)) == 0) {
		return true;
	}
	else {
		return false;
	}
#endif
}

int
platform_tcp_create(void) {
#ifdef _COBWEB_WIN32
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	return sockfd;
#elif _COBWEB_LINUX
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	return sockfd;
#endif
}

bool
platform_tcp_listen(int sockfd) {
#ifdef _COBWEB_WIN32
	return listen(sockfd, SOMAXCONN) == 0 ? true : false;
#elif _COBWEB_LINUX
	return listen(sockfd, SOMAXCONN) == 0 ? true : false;
#endif
}

struct sockaddr_t
	platform_tcp_accept(int sockfd) {
	struct sockaddr_t sockaddr;
	memset(&sockaddr, 0, sizeof(struct sockaddr_t));
#ifdef _COBWEB_WIN32
	struct sockaddr_in client;
	int len = sizeof(client);
	int clientfd = accept(sockfd, (struct sockaddr*)&client, &len);
	sockaddr.sockfd = clientfd;
	strncpy(sockaddr.ip, inet_ntoa(client.sin_addr), CC_MAX_IP);
	sockaddr.port = ntohs(client.sin_port);
#elif _COBWEB_LINUX
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	int clientfd = accept(sockfd, (struct sockaddr*)&client, &len);
	sockaddr.sockfd = clientfd;
	strncpy(sockaddr.ip, inet_ntoa(client.sin_addr), CC_MAX_IP);
	sockaddr.port = ntohs(client.sin_port);
#endif
	return sockaddr;
}

int
platform_tcp_send(int sockfd, const char* buf, int len) {
#ifdef _COBWEB_WIN32
	int n = send(sockfd, buf, len, 0);
	return n;
#elif _COBWEB_LINUX
	int n = send(sockfd, buf, len, 0);
	return n;
#endif
}

int
platform_tcp_recv(int sockfd, char* buffer) {
#ifdef _COBWEB_WIN32
	char buf[CC_MAX_BUFFER] = { 0 };
	memset(buf, 0, CC_MAX_BUFFER);
	int size = recv(sockfd, buf, CC_MAX_BUFFER, 0);
	if (size > 0) {
		memcpy(buffer, buf, size);
	}
	return size;
#elif _COBWEB_LINUX
	char buf[CC_MAX_BUFFER] = { 0 };
	memset(buf, 0, CC_MAX_BUFFER);
	int size = recv(sockfd, buf, CC_MAX_BUFFER, 0);
	if (size > 0) {
		memcpy(buffer, buf, size);
	}
	return size;
#endif
}

int
platform_udp_create(void) {
#ifdef _COBWEB_WIN32
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	return sockfd;
#elif _COBWEB_LINUX
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	return sockfd;
#endif
}

int
platform_udp_recv(int sockfd, char* out_buffer, char* ip, int* port) {
#ifdef _COBWEB_WIN32
	struct sockaddr_in clientaddr;
	int len = sizeof(clientaddr);
	char buffer[CC_MAX_BUFFER] = { 0 };
	int size = recvfrom(sockfd, buffer, CC_MAX_BUFFER, 0, (struct sockaddr*)&clientaddr, &len);
	if (size > 0) {
		memcpy(out_buffer, buffer, size);
	}
	strcpy(ip, inet_ntoa(clientaddr.sin_addr));
	*port = ntohs(clientaddr.sin_port);
	return size;
#elif _COBWEB_LINUX
	struct sockaddr_in clientaddr;
	int len = sizeof(clientaddr);
	char buffer[CC_MAX_BUFFER] = { 0 };
	int size = recvfrom(sockfd, buffer, CC_MAX_BUFFER, 0, (struct sockaddr*)&clientaddr, &len);
	if (size > 0) {
		memcpy(out_buffer, buffer, size);
	}
	strcpy(ip, inet_ntoa(clientaddr.sin_addr));
	*port = ntohs(clientaddr.sin_port);
	return size;
#endif
}

int
platform_udp_send(int sockfd, const char* buffer, int len,
	const char* ip, int port) {
#ifdef _COBWEB_WIN32
	struct sockaddr_in addr = platform_sockaddr(ip, port);
	int n = sendto(sockfd, buffer, len, 0, (struct sockaddr*)&addr, sizeof(addr));
	return n;
#elif _COBWEB_LINUX
	struct sockaddr_in addr = platform_sockaddr(ip, port);
	int n = sendto(sockfd, buffer, len, 0, (struct sockaddr*)&addr, sizeof(addr));
	return n;
#endif
}

void
platform_msleep(int msec) {
#ifdef _COBWEB_WIN32
	Sleep(msec);
#elif _COBWEB_LINUX
	usleep((msec << 10) - (msec << 4) - (msec << 3));
#endif
}

bool
platform_file_exists(const char* file) {
#ifdef _COBWEB_WIN32
	WIN32_FIND_DATAA wfd;
	HANDLE hFind = FindFirstFileA(file, &wfd);
	return hFind != INVALID_HANDLE_VALUE ? true : false;
#elif _COBWEB_LINUX
	return file != NULL ? true : false;
#endif
}

char*
platform_root_dir(char* path, int len) {
#ifdef _COBWEB_WIN32
	memset(path, 0, len);
	GetCurrentDirectoryA((int)len, path);
#elif _COBWEB_LINUX
	memset(path, 0, len);
	getcwd(path, len);
#endif
	return path;
}

struct datetime_t
platform_datetime(const char* format) {
	struct datetime_t datetime;
	time_t rawtime;
	time(&rawtime);
	struct tm* time = (struct tm*)localtime(&rawtime);
	strftime(datetime.buffer, 64, format, time);
	return datetime;
}

void
platform_timeofday(long* sec, long* usec) {
#ifdef _COBWEB_WIN32
	struct tm tm;
	SYSTEMTIME wtm;
	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	*sec = (long)mktime(&tm);
	*usec = wtm.wMilliseconds * 1000;
#elif _COBWEB_LINUX
	struct timeval time;
	gettimeofday(&time, NULL);
	if (sec) {
		*sec = time.tv_sec;
	}
	if (usec) {
		*usec = time.tv_usec;
	}
#endif
}

void*
platform_dlopen(const char* file) {
#ifdef _COBWEB_WIN32
	void* dl = (void*)LoadLibraryA(file);
#elif _COBWEB_LINUX
	void* dl = dlopen(file, RTLD_NOW | RTLD_GLOBAL);
#endif
	return dl;
}

void*
platform_dlsym(void* dl, const char* name) {
#ifdef _COBWEB_WIN32
	void* address = (void*)GetProcAddress((HMODULE)dl, name);
#elif _COBWEB_LINUX
	void* address = dlsym(dl, name);
#endif
	return address;
}

int
platform_dlclose(void* dl) {
#ifdef _COBWEB_WIN32
	bool ret = FreeLibrary((HMODULE)dl);
#elif _COBWEB_LINUX
	bool ret = (dlclose(dl) == 0);
#endif
	return ret;
}

void
platform_dlerror(char* msg) {
#ifdef _COBWEB_WIN32
	int errorno = GetLastError();
	sprintf(msg, "error number is %d", errorno);
#elif _COBWEB_LINUX
	sprintf(msg, "%s", dlerror());
#endif
}

void
_color_print(enum FG_COLOR_E color, const char* str) {
#ifdef _COBWEB_WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	printf("%s\n", str);
#elif _COBWEB_LINUX
	printf("\033[%d;%dm%s\033[0m\n", BG_NONE, color, str);
#endif
}

void
platform_green_print(const char* str) {
	_color_print(FG_GREEN, str);
}

void
platform_blue_print(const char* str) {
	_color_print(FG_BLUE, str);
}

void
platform_red_print(const char* str) {
	_color_print(FG_RED, str);
}

int
platform_log(const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	char tmp[CC_MESSAGE_SIZE];
	memset(tmp, 0, CC_MESSAGE_SIZE);
	int len = vsnprintf(tmp, CC_MESSAGE_SIZE, format, ap);
	va_end(ap);
	char* data = NULL;
	if (len >= 0 && len < CC_MESSAGE_SIZE) {
		data = (char*)malloc(CC_MESSAGE_SIZE);
		if (data != NULL) {
			memset(data, 0, CC_MESSAGE_SIZE);
			memcpy(data, tmp, len);
		}
	}
	else {
		int max_size = CC_MESSAGE_SIZE;
		for (;;) {
			max_size *= 2;
			data = (char*)malloc(max_size);
			if (data != NULL) {
				memset(data, 0, max_size);
				va_start(ap, format);
				len = vsnprintf(data, max_size, format, ap);
				va_end(ap);
				if (len < max_size) {
					break;
				}
				free(data);
			}
		}
	}
	platform_green_print(data);
	free(data);
	return len;
}

int
platform_debug(const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	char tmp[CC_MESSAGE_SIZE];
	memset(tmp, 0, CC_MESSAGE_SIZE);
	int len = vsnprintf(tmp, CC_MESSAGE_SIZE, format, ap);
	va_end(ap);
	char* data = NULL;
	if (len >= 0 && len < CC_MESSAGE_SIZE) {
		data = (char*)malloc(CC_MESSAGE_SIZE);
		if (data != NULL) {
			memset(data, 0, CC_MESSAGE_SIZE);
			memcpy(data, tmp, len);
		}
	}
	else {
		int max_size = CC_MESSAGE_SIZE;
		for (;;) {
			max_size *= 2;
			data = (char*)malloc(max_size);
			if (data != NULL) {
				memset(data, 0, max_size);
				va_start(ap, format);
				len = vsnprintf(data, max_size, format, ap);
				va_end(ap);
				if (len < max_size) {
					break;
				}
				free(data);
			}
		}
	}
	platform_blue_print(data);
	free(data);
	return len;
}

int
platform_error(const char* format, ...) {
	va_list ap;
	va_start(ap, format);
	char tmp[CC_MESSAGE_SIZE];
	memset(tmp, 0, CC_MESSAGE_SIZE);
	int len = vsnprintf(tmp, CC_MESSAGE_SIZE, format, ap);
	va_end(ap);
	char* data = NULL;
	if (len >= 0 && len < CC_MESSAGE_SIZE) {
		data = (char*)malloc(CC_MESSAGE_SIZE);
		if (data != NULL) {
			memset(data, 0, CC_MESSAGE_SIZE);
			memcpy(data, tmp, len);
		}
	}
	else {
		int max_size = CC_MESSAGE_SIZE;
		for (;;) {
			max_size *= 2;
			data = (char*)malloc(max_size);
			if (data != NULL) {
				memset(data, 0, max_size);
				va_start(ap, format);
				len = vsnprintf(data, max_size, format, ap);
				va_end(ap);
				if (len < max_size) {
					break;
				}
				free(data);
			}
		}
	}
	platform_red_print(data);
	free(data);
	return len;
}

struct thread_t
platform_thread_create(void* (*start_rtn)(void*), void* arg) {
	struct thread_t th = { 0 };
#ifdef _COBWEB_WIN32
	th.handle = _beginthreadex(NULL, 0, (_beginthreadex_proc_type)start_rtn, arg, 0, (unsigned int *)(& th.tid));
#elif _COBWEB_LINUX
	pthread_create(&th.handle, NULL, start_rtn, arg);
	th.tid = th.handle;
#endif
	return th;
}

bool
platform_thread_wait(struct thread_t* pth) {
#ifdef _COBWEB_WIN32
	DWORD ret = WaitForSingleObject((HANDLE)pth->handle, -1);
	return ret != WAIT_OBJECT_0 ? true : false;
#elif _COBWEB_LINUX
	void* status;
	int ret = pthread_join(pth->handle, &status);
	return ret == 0 ? true : false;
#endif
}

bool
platform_thread_release(struct thread_t* pth) {
#ifdef _COBWEB_WIN32
	return CloseHandle((HANDLE)pth->handle) ? true : false;
#elif _COBWEB_LINUX
	return pthread_cancel(pth->handle) == 0 ? true : false;
#endif
}

int
platform_gettid(void) {
#ifdef _COBWEB_WIN32
	return GetCurrentThreadId();
#elif _COBWEB_LINUX
	return (int)pthread_self();
#endif
}

struct mutex_t*
	platform_mutex_create(void) {
	struct mutex_t* mutex = (struct mutex_t*)malloc(sizeof(struct mutex_t));
	assert(mutex != NULL);
#ifdef _COBWEB_WIN32
	InitializeCriticalSectionAndSpinCount(&mutex->m, 0x80000400);
#elif _COBWEB_LINUX
	pthread_mutex_init(&mutex->m, NULL);
#endif
	return mutex;
}

void
platform_mutex_lock(struct mutex_t* mutex) {
#ifdef _COBWEB_WIN32
	EnterCriticalSection(&mutex->m);
#elif _COBWEB_LINUX
	pthread_mutex_lock(&mutex->m);
#endif
}

void
platform_mutex_unlock(struct mutex_t* mutex) {
#ifdef _COBWEB_WIN32
	LeaveCriticalSection(&mutex->m);
#elif _COBWEB_LINUX
	pthread_mutex_unlock(&mutex->m);
#endif
}

void
platform_mutex_release(struct mutex_t* mutex) {
	if (mutex != NULL) {
#ifdef _COBWEB_WIN32
		DeleteCriticalSection(&mutex->m);
#elif _COBWEB_LINUX
		pthread_mutex_destroy(&mutex->m);
#endif
		free(mutex);
	}
}

/* create event */
struct event_t*
platform_event_create(const char* name, struct mutex_t* mutex) {
	struct event_t* event = (struct event_t*)malloc(sizeof(struct event_t));
	assert(event != NULL);
#ifdef _COBWEB_WIN32
	strcpy(event->name, name);
	event->mutex = mutex;
	event->handle = CreateEventA(NULL, true, false, name);
#elif _COBWEB_LINUX
	strcpy(event->name, name);
	event->mutex = mutex;
#endif
	return event;
}

/* wait event */
void
platform_event_wait(struct event_t* event) {
#ifdef _COBWEB_WIN32
	WaitForSingleObject(event->handle, -1);
#elif _COBWEB_LINUX
	pthread_cond_wait(&event->handle, &event->mutex->m);
#endif
}

/* signal event */
void
platform_event_signal(struct event_t* event) {
#ifdef _COBWEB_WIN32
	SetEvent(event->handle);
#elif _COBWEB_LINUX
	pthread_cond_signal(&event->handle);
#endif
}

/* release event */
void
platform_event_release(struct event_t* event) {
	if (event != NULL) {
#ifdef _COBWEB_WIN32
		CloseHandle(event->handle);
#elif _COBWEB_LINUX
		pthread_cond_destroy(&event->handle);
#endif
		free(event);
		event = NULL;
	}
}

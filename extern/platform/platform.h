#ifndef _PLATFORM_H
#define _PLATFORM_H


#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


/* datetime struct */
struct datetime_t {
	char buffer[64];
};

/* socket address */
struct sockaddr_t {
	int sockfd;
	char ip[64];
	int port;
};

/* thread */
struct thread_t {
	unsigned long handle;
	unsigned long int tid;
};

/* mutex struct */
struct mutex_t;

/* event struct */
struct event_t;


/* std */
bool ccp_network_init(void);
bool ccp_network_release(void);
int ccp_anyaddr(void);
bool ccp_socket_close(int);
struct sockaddr_in ccp_sockaddr(const char*, int);
bool ccp_socket_nonblock(int);
bool ccp_socket_bind(int, const char*, int);
int ccp_tcp_create(void);
bool ccp_tcp_connect(int, const char*, int);
bool ccp_tcp_listen(int);
struct sockaddr_t ccp_tcp_accept(int);
int ccp_tcp_send(int, const char*, int);
int ccp_tcp_recv(int, char*);
int ccp_udp_create(void);
int ccp_udp_recv(int, char*, char*, int*);
int ccp_udp_send(int, const char*, int, const char*, int);

void ccp_msleep(int);
bool ccp_file_exists(const char* file);
char* ccp_root_dir(char*, int);
struct datetime_t ccp_datetime(const char* format);
/* 1971.1.1 to today */
void ccp_timeofday(long*, long*);
void* ccp_dlopen(const char*);
void* ccp_dlsym(void*, const char*);
int ccp_dlclose(void*);
void ccp_dlerror(char*);

void ccp_green_print(const char*);
void ccp_blue_print(const char*);
void ccp_red_print(const char*);
int ccp_log(const char* format, ...);
int ccp_debug(const char* format, ...);
int ccp_error(const char* format, ...);

struct thread_t ccp_thread_create(void* (*start_rtn)(void*), void* arg);
bool ccp_thread_wait(struct thread_t*);
bool ccp_thread_release(struct thread_t*);
int ccp_gettid(void);
struct mutex_t* ccp_mutex_create(void);
void ccp_mutex_lock(struct mutex_t*);
void ccp_mutex_unlock(struct mutex_t*);
void ccp_mutex_release(struct mutex_t*);
struct event_t* ccp_event_create(const char*, struct mutex_t*);
void ccp_event_wait(struct event_t*);
void ccp_event_signal(struct event_t*);
void ccp_event_release(struct event_t*);

/* linux */
int ccp_daemon_init(const char*);
int ccp_daemon_exit(const char*);
void ccp_exception_debug(int);
int ccp_timefd_create(void);
int ccp_timer_timeout(int);
void ccp_timerfd_close(int);

#endif

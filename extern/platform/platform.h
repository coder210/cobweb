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
bool platform_network_init(void);
bool platform_network_release(void);
int platform_anyaddr(void);
bool platform_socket_close(int);
struct sockaddr_in platform_sockaddr(const char*, int);
bool platform_socket_nonblock(int);
bool platform_socket_bind(int, const char*, int);
int platform_tcp_create(void);
bool platform_tcp_connect(int, const char*, int);
bool platform_tcp_listen(int);
struct sockaddr_t platform_tcp_accept(int);
int platform_tcp_send(int, const char*, int);
int platform_tcp_recv(int, char*);
int platform_udp_create(void);
int platform_udp_recv(int, char*, char*, int*);
int platform_udp_send(int, const char*, int, const char*, int);

void platform_msleep(int);
bool platform_file_exists(const char* file);
char* platform_root_dir(char*, int);
struct datetime_t platform_datetime(const char* format);
/* 1971.1.1 to today */
void platform_timeofday(long*, long*);
void* platform_dlopen(const char*);
void* platform_dlsym(void*, const char*);
int platform_dlclose(void*);
void platform_dlerror(char*);

void platform_green_print(const char*);
void platform_blue_print(const char*);
void platform_red_print(const char*);
int platform_log(const char* format, ...);
int platform_debug(const char* format, ...);
int platform_error(const char* format, ...);

struct thread_t platform_thread_create(void* (*start_rtn)(void*), void* arg);
bool platform_thread_wait(struct thread_t*);
bool platform_thread_release(struct thread_t*);
int platform_gettid(void);
struct mutex_t* platform_mutex_create(void);
void platform_mutex_lock(struct mutex_t*);
void platform_mutex_unlock(struct mutex_t*);
void platform_mutex_release(struct mutex_t*);
struct event_t* platform_event_create(const char*, struct mutex_t*);
void platform_event_wait(struct event_t*);
void platform_event_signal(struct event_t*);
void platform_event_release(struct event_t*);

/* linux */
int platform_daemon_init(const char*);
int platform_daemon_exit(const char*);
void platform_exception_debug(int);
int platform_timefd_create(void);
int platform_timer_timeout(int);
void platform_timerfd_close(int);

#endif

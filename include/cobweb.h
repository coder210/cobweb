#ifndef _COBWEB_H
#define _COBWEB_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include <malloc.h>
#include "platform/platform.h"


#define cobweb_malloc malloc
#define cobweb_malloca _malloca
#define cobweb_alloca alloca
#define cobweb_calloc calloc
#define cobweb_realloc realloc
#define cobweb_free free

/* 一个包建议长度 */
#define COBWEB_MESSAGE_SIZE 2048
/* 一个包最大长度,超过就会直接丢弃 */
#define COBWEB_MAX_SIZE (COBWEB_MESSAGE_SIZE * 2)
#define COBWEB_MAX_BUFFER 1024
#define COBWEB_MAX_PATH 255
#define COBWEB_MAX_IP 128
#define COBWEB_CMD_SIZE 64 
#define COBWEB_BACKTRACE_SIZE 128
#define COBWEB_MAX_IDSTR_SIZE 64 
#define COBWEB_MAX_DATE_LEN 16

#define PTYPE_TEXT 0
#define PTYPE_RESPONSE 1
#define PTYPE_MULTICAST 2
#define PTYPE_CLIENT 3
#define PTYPE_SYSTEM 4
#define PTYPE_HARBOR 5
#define PTYPE_SOCKET 6
#define PTYPE_ERROR 7
#define PTYPE_RESERVED_QUEUE 8
#define PTYPE_RESERVED_DEBUG 9
#define PTYPE_RESERVED_LUA 10
#define PTYPE_RESERVED_SNAX 11
#define PTYPE_TAG_DONTCOPY 0x10000
#define PTYPE_TAG_ALLOCSESSION 0x20000

#define SOCKET_TYPE_DATA 101
#define SOCKET_TYPE_CONNECT 102
#define SOCKET_TYPE_CLOSE 103
#define SOCKET_TYPE_ACCEPT 104
#define SOCKET_TYPE_ERROR 105
#define SOCKET_TYPE_WARNING 106

#define STATE_WAIT 0
#define STATE_DISPATCH 1
#define STATE_SUSPEND 2
#define STATE_EXIT 3
#define STATE_CLEAR 99


/* socket message struct */
struct socket_message_t {
	int type; // 类型
	int fd; // socket fd
	size_t sz; //
	char buffer[COBWEB_MESSAGE_SIZE]; // data
};

/* message struct */ 
struct message_t {
	int type;
	uint32_t source;
	int session;
	void* data;
	size_t sz;
};

/* config */
struct config_t {
	int thread;
	int harbor;
	const char* daemon;
	const char* module_path;
	const char* bootstrap;
	const char* logservice;
	const char* address;
	const char* master;
	int afterend;
	int proto_parse;
};

/* module struct */
struct module_t;

/* monitor struct */
struct monitor_t;

/* mq struct */
struct mq_t;

/* context struct */
struct context_t;

/* context callback */
typedef int (*context_callback)(struct context_t* ctx, void* ud, int type, int session, uint32_t source, const void* msg, size_t sz);

typedef int (*api_sserver_send)(int fd, const char* buffer, size_t sz);
typedef int (*api_context_send)(struct context_t* ctx, uint32_t source, uint32_t dest, int type, int session, void* data, size_t sz);
typedef int (*api_context_sendname)(struct context_t* ctx, uint32_t source, const char* addr, int type, int session, void* data, size_t sz);
typedef void (*api_context_callback)(struct context_t* ctx, void* ud, context_callback cb);
typedef const char* (*api_context_command)(struct context_t* ctx, const char* cmd, const char* param);
typedef void (*api_context_log)(struct context_t*, const char*, ...);
typedef void (*api_context_error)(struct context_t*, const char*, ...);

struct context_data_t;

// ctx
struct context_t {
	struct context_data_t* _data;
	api_sserver_send sserver_send;
	api_context_send send;
	api_context_sendname sendname;
	api_context_callback callback;
	api_context_command command;
	api_context_log log;
	api_context_error error;
};

/* linkedlist_cb */
typedef bool(*linkedlist_cb)(void* data, void* arg);

/* context */
void cobweb_context_endless(struct context_t* ctx);
bool cobweb_context_action(struct context_t* ctx, struct monitor_t*);
uint32_t cobweb_context_handle(struct context_t* ctx);
int cobweb_context_newsession(struct context_t* ctx);
struct context_t* cobweb_context_new(const char* name, const char* param);
void cobweb_context_release(struct context_t* ctx);
int cobweb_context_push(uint32_t handle, struct message_t* message);
int cobweb_context_ssend(struct socket_message_t* socket_message);
int cobweb_context_send(struct context_t* ctx, uint32_t source, uint32_t dest, int type, int session, void* data, size_t sz);
int cobweb_context_sendname(struct context_t* ctx, uint32_t source, const char* addr, int type, int session, void* data, size_t sz);
void cobweb_context_callback(struct context_t* ctx, void* ud, context_callback cb);
const char* cobweb_context_command(struct context_t* ctx, const char* cmd, const char* param);
void cobweb_context_log(struct context_t*, const char*, ...);
void cobweb_context_error(struct context_t*, const char*, ...);
void cobweb_context_log_output(FILE*, uint32_t, int, int, void*, size_t);

/* handle */
void cobweb_handle_init(int harbor);
uint32_t cobweb_handle_register(struct context_t* ctx);
int cobweb_handle_retire(uint32_t handle);
struct context_t* cobweb_handle_grab(uint32_t handle);
uint32_t cobweb_handle_findname(const char* name);
const char* cobweb_handle_namehandle(uint32_t handle, const char* name);
void cobweb_handle_release();

/* infra */
struct linkedlist_t* cobweb_linkedlist_create();
void* cobweb_linkedlist_query(struct linkedlist_t* list, linkedlist_cb f, void* param);
void cobweb_linkedlist_each(struct linkedlist_t* list, linkedlist_cb f, void* param);
bool cobweb_linkedlist_contain(struct linkedlist_t* list, void* data);
bool cobweb_linkedlist_containf(struct linkedlist_t* list, linkedlist_cb f, void* param);
int cobweb_linkedlist_length(struct linkedlist_t* list);
struct node_t* cobweb_linkedlist_insert(struct linkedlist_t* list, void* data);
bool cobweb_linkedlist_removef(struct linkedlist_t* list, linkedlist_cb f, void* param);
bool cobweb_linkedlist_remove(struct linkedlist_t* list, void* data);
bool cobweb_linkedlist_drainf(struct linkedlist_t* list, linkedlist_cb f, void* param);
bool cobweb_linkedlist_drain(struct linkedlist_t* list, void* data);
void cobweb_linkedlist_release(struct linkedlist_t* list);
struct queue_t* cobweb_queue_create();
bool cobweb_queue_contain(struct queue_t* queue, void* data);
void* cobweb_queue_first(struct queue_t* queue);
void cobweb_eq(struct queue_t* queue, void* data);
void cobweb_eqd(struct queue_t* queue, void* data);
void* cobweb_dq(struct queue_t* queue);
int cobweb_queue_length(struct queue_t* queue);
void cobweb_queue_release(struct queue_t* queue);

/* module */
void cobweb_module_init(const char*);
void cobweb_module_release(void);
void cobweb_module_insert(struct module_t*);
struct module_t* cobweb_module_query(const char*);
void* cobweb_module_instance_create(struct module_t*);
int cobweb_module_instance_init(struct module_t*, void*, struct context_t*, const char*);
void cobweb_module_instance_release(struct module_t*, void*);
void cobweb_module_instance_signal(struct module_t*, void*, int);

/* monitor */ 
void cobweb_monitor_init(int);
void cobweb_monitor_release(void);
void cobweb_monitor_trigger(struct monitor_t*, uint32_t, uint32_t);
bool cobweb_monitor_check(struct monitor_t*);
void cobweb_monitor_error(struct monitor_t*);
bool cobweb_monitor_quit(void);
int cobweb_monitor_count(void);
struct monitor_t* cobweb_query_monitor(int);
void cobweb_monitor_wait(void);
void cobweb_monitor_wakeup(int);

/* socket server */
void cobweb_sserver_init(char*, int, int);
int cobweb_sserver_dispatch(void);
void cobweb_sserver_loopexit(void);
int cobweb_sserver_send(int, const char*, size_t);
void cobweb_sserver_release(void);

/* kcp server */
struct kcpserver_t* cobweb_kcpserver_create(const char*, int);
int cobweb_kcpserver_send(struct kcpserver_t*, int, const char*, size_t);
void cobweb_kcpserver_dispatch(struct kcpserver_t*);
void cobweb_kcpserver_loopexit(struct kcpserver_t*);
void cobweb_kcpserver_release(struct kcpserver_t*);

/* tcp server */
struct tcpserver_t* cobweb_tcpserver_create(const char*, int, int);
int cobweb_tcpserver_send(struct tcpserver_t*, int, const char*, size_t);
void cobweb_tcpserver_dispatch(struct tcpserver_t*);
void cobweb_tcpserver_loopexit(struct tcpserver_t*);
void cobweb_tcpserver_release(struct tcpserver_t*);

/* global mq */
void cobweb_globalmq_init(void);
void cobweb_globalmq_release(void);
int cobweb_globalmq_length(void);
void cobweb_globalmq_eq(struct mq_t*);
struct mq_t* cobweb_globalmq_dq(void);
uint32_t cobweb_mq_handle(struct mq_t*);
struct mq_t* cobweb_mq_create(uint32_t);
void cobweb_mq_release(struct mq_t*);
int cobweb_mq_length(struct mq_t*);
struct message_t* cobweb_mq_dq(struct mq_t*);
void cobweb_mq_eq(struct mq_t*, struct message_t*);

/* start */
int cobweb_run(const char* config_file);
void cobweb_start(struct config_t*);

/* timer */
void cobweb_timer_init(void);
int cobweb_add_wheel(uint32_t, int, int, void*, size_t);
bool cobweb_del_wheel(int);
int cobweb_timer_tick(bool(*tick)(uint32_t handle, int session, void* data, size_t sz, void* arg), void*);
void cobweb_timer_release(void);

/* utils */
char* cobweb_strsep(char**, const char*);
char* cobweb_read_file(const char*);
void cobweb_append_file(const char*, const char*);
int64_t cobweb_timestamp(void);
char* cobweb_strdup(const char*);
void cobweb_write_short(uint8_t*, short);
short cobweb_read_short(uint8_t*);
void cobweb_write_int(uint8_t*, int);
int cobweb_read_int(uint8_t*);

/* env */
void cobweb_env_init(void);
const char* cobweb_getenv(const char*);
void cobweb_setenv(const char*, const char*);
void cobweb_env_release(void);



#endif

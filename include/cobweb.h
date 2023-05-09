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

#define COBWEB_MESSAGE_SIZE 2048
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

#define PTYPE_TAG_DONTCOPY 0x10000
#define PTYPE_TAG_ALLOCSESSION 0x20000

#define SOCKET_TYPE_DATA 101
#define SOCKET_TYPE_CONNECT 102
#define SOCKET_TYPE_CLOSE 103
#define SOCKET_TYPE_ACCEPT 104
#define SOCKET_TYPE_ERROR 105
#define SOCKET_TYPE_WARNING 106


struct message_t {
	int type;
	uint32_t source;
	int session;
	void* data;
	size_t sz;
};

struct config_t {
	int thread;
	const char* daemon;
	const char* module_path;
	const char* bootstrap;
	const char* logservice;
	const char* address;
	const char* master;
};

struct context_t;

/* context callback */
typedef int (*context_callback)(struct context_t* ctx, void* ud, int type, int session, uint32_t source, const void* msg, size_t sz);

/* api */
typedef int (*api_context_send)(struct context_t* ctx, uint32_t source, uint32_t dest, int type, int session, void* data, size_t sz);
typedef int (*api_context_sendname)(struct context_t* ctx, uint32_t source, const char* addr, int type, int session, void* data, size_t sz);
typedef void (*api_context_callback)(struct context_t* ctx, void* ud, context_callback cb);
typedef const char* (*api_context_command)(struct context_t* ctx, const char* cmd, const char* param);
typedef void (*api_context_log)(struct context_t*, const char*, ...);
typedef void (*api_context_error)(struct context_t*, const char*, ...);

struct context_data_t;

struct context_t {
	struct context_data_t* _data;
	api_context_send send;
	api_context_sendname sendname;
	api_context_callback callback;
	api_context_command command;
	api_context_log log;
	api_context_error error;
};

/* context */
void cobweb_context_endless(struct context_t* ctx);
uint32_t cobweb_context_handle(struct context_t* ctx);
int cobweb_context_newsession(struct context_t* ctx);
struct context_t* cobweb_context_new(const char* name, const char* param);
void cobweb_context_release(struct context_t* ctx);
int cobweb_context_push(uint32_t handle, struct message_t& msg);
int cobweb_context_send(struct context_t* ctx, uint32_t source, uint32_t dest, int type, int session, void* data, size_t sz);
int cobweb_context_sendname(struct context_t* ctx, uint32_t source, const char* addr, int type, int session, void* data, size_t sz);
void cobweb_context_callback(struct context_t* ctx, void* ud, context_callback cb);
const char* cobweb_context_command(struct context_t* ctx, const char* cmd, const char* param);
void cobweb_context_log(struct context_t*, const char*, ...);
void cobweb_context_error(struct context_t*, const char*, ...);
void cobweb_context_log_output(FILE*, uint32_t, int, int, void*, size_t);
bool cobweb_dispatch_message(struct monitor_t*, struct context_t*);

/* handle */
void cobweb_handle_init(void);
uint32_t cobweb_handle_register(struct context_t* ctx);
int cobweb_handle_retire(uint32_t handle);
struct context_t* cobweb_handle_find(uint32_t handle);
uint32_t cobweb_handle_findname(const char* name);
const char* cobweb_handle_namehandle(uint32_t handle, const char* name);
void cobweb_handle_release();

/* module */
void cobweb_module_init(const char*);
void cobweb_module_release(void);
struct module_t* cobweb_module_query(const char*);
void* cobweb_module_instance_create(struct module_t*);
int cobweb_module_instance_init(struct module_t*, void*, struct context_t*, const char*);
void cobweb_module_instance_release(struct module_t*, void*);
void cobweb_module_instance_signal(struct module_t*, void*, int);

/* monitor */ 
void cobweb_monitor_init(void);
void cobweb_monitor_release(void);
struct monitor_t* cobweb_monitor_new(void);
void cobweb_monitor_trigger(struct monitor_t*, uint32_t, uint32_t);
void cobweb_monitor_check(void);
void cobweb_monitor_sleep(void);
bool cobweb_monitor_quit(void);
void cobweb_monitor_wait(void);
void cobweb_monitor_wakeup(void);

/* context queue */
void cobweb_cq_init(void);
void cobweb_cq_eq(struct context_t* ctx);
struct context_t* cobweb_cq_dq();
void cobweb_cq_release();

/* start */
int cobweb_run(const char* config_file);
void cobweb_start(struct config_t*);

/* timer */
void cobweb_timer_init(void);
int cobweb_add_trigger(uint32_t, int, int, void*, size_t);
int cobweb_timer_tick(void*);
void cobweb_timer_sleep(void);
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

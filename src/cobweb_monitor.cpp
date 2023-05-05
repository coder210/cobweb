/********************************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_monitor.c
Description: 监控消息分发时的状态,出现异常进行预警
Author: ydlc
Version: 1.0
Date: 2021.12.10
History:
********************************************************/

#include "cobweb.h"


struct monitor_t {
	int version; //版本号
	int check_version;  //上个版本号
	uint32_t source;  //源
	uint32_t destination;  //目标
};


struct monitors_t {
	int count;
	struct monitor_t** ms;
	struct mutex_t* mutex;
	struct event_t* event;
	int sleep;
	bool quit;
};


static struct monitors_t* M = NULL;

static struct monitor_t*
_create_monitor() {
	struct monitor_t* sm = (struct monitor_t*)cobweb_malloc(sizeof(struct monitor_t));
	if (sm != NULL) {
		memset(sm, 0, sizeof(struct monitor_t));
	}
	return sm;
}

static void
_destory_monitor(struct monitor_t* sm) {
	if (sm != NULL) {
		cobweb_free (sm);
	}
}

/* 结构体初始化 */
void
cobweb_monitor_init(int count) {
	M = (struct monitors_t*)cobweb_malloc(sizeof(struct monitors_t));
	if (M != NULL) {
		memset(M, 0, sizeof(struct monitors_t));
		M->count = count;
		M->sleep = 0;
		M->quit = false;
		M->ms = (struct monitor_t**)cobweb_malloc(count * sizeof(struct monitor_t*));
		if (M->ms != NULL) {
			for (int i = 0; i < count; i++) {
				M->ms[i] = _create_monitor();
			}
		}
		M->mutex = ccp_mutex_create();
		M->event = ccp_event_create("MONITOR", M->mutex);
	}
}

void
cobweb_monitor_release(void) {
	if (M != NULL) {
		int count = M->count;
		for (int i = 0; i < count; i++) {
			_destory_monitor(M->ms[i]);
		}

		ccp_mutex_release(M->mutex);
		ccp_event_release(M->event);
		cobweb_free (M->ms);
		cobweb_free (M);
	}
}

//触发监控
void
cobweb_monitor_trigger(struct monitor_t* sm, uint32_t source, uint32_t destination) {
	sm->source = source;
	sm->destination = destination;

	ccp_mutex_lock(M->mutex);
	sm->version++;
	ccp_mutex_unlock(M->mutex);
}

// 检查监控，有一个线程专门做这个事情
bool
cobweb_monitor_check(struct monitor_t* sm) {
	if (sm->version == sm->check_version) {
		if (sm->destination) {
			return false;
		}
	}
	else {
		sm->check_version = sm->version;
	}

	return true;
}

void
cobweb_monitor_error(struct monitor_t* sm) {
	struct context_t* ctx = cobweb_handle_grab(sm->destination);
	cobweb_context_error(ctx, "A message from [ :%08x ] to [ :%08x ] maybe in an endless loop (version = %d)", sm->source, sm->destination, sm->version);
}

bool
cobweb_monitor_quit(void) {
	return M->quit;
}

int
cobweb_monitor_count(void) {
	return M->count;
}

struct monitor_t* 
cobweb_query_monitor(int index) {
	struct monitor_t* sm = M->ms[index];
	return sm;
}

void
cobweb_monitor_wait(void) {
	ccp_mutex_lock(M->mutex);
	++M->sleep;
	if (!M->quit) {
		ccp_event_wait(M->event);
	}
	--M->sleep;
	ccp_mutex_unlock(M->mutex);
}

void
cobweb_monitor_wakeup(int busy) {
	if (M->sleep >= busy) {
		// signal sleep worker, "spurious wakeup" is harmless
		ccp_event_signal(M->event);
	}
}

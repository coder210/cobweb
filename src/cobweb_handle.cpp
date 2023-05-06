/***************************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_handle.c
Description: 所有的服务在这里通过handle进行模询到
Author: ydlc
Version: 1.0
Date: 2021.10.10
History:
****************************************************/

#include "cobweb.h"
#include <assert.h>


// reserve high 8 bits for remote id
#define HANDLE_MASK 0xffffff
#define HANDLE_REMOTE_SHIFT 24
#define DEFAULT_SLOT_SIZE 1024
#define MAX_NAME_SIZE 32


//名字服务结构，一个名字对应一个handle
struct handle_name_t {
	char name[MAX_NAME_SIZE]; //服务名字
	uint32_t handle; //服务ID，下面以handle来称呼
};


//保存handle/name列表的数据结构，skynet_handle_init会初始化它
struct handle_storage_t {
	struct mutex_t* mutex;
	uint32_t harbor; //harbor!!
	uint32_t handle_index;  //必须从1开始
	size_t slot_size; //数组长度
	struct context_t** slot;  //数组，实际上里面存的是服务的上下文
	struct handle_name_t* name;  //数组
};


static struct handle_storage_t* H = NULL;


uint32_t
cobweb_handle_register(struct context_t* ctx) {
	uint32_t handle = 0;
	platform_mutex_lock(H->mutex);
	handle = H->handle_index++;
	H->slot[handle] = ctx;
	platform_mutex_unlock(H->mutex);
	return handle;
}

int
cobweb_handle_retire(uint32_t handle) {
	struct context_t* ctx = H->slot[handle];
	if (ctx == NULL) {
		return 1;
	}

	platform_mutex_lock(H->mutex);
	H->slot[handle] = NULL;
	H->name[handle].handle = 0;
	memset(H->name[handle].name, 0, MAX_NAME_SIZE);
	platform_mutex_unlock(H->mutex);

	return 0;
}

struct context_t*
cobweb_handle_grab(uint32_t handle) {
	struct context_t* ctx = H->slot[handle];
	if (ctx != NULL && cobweb_context_handle(ctx) == handle) {
		return ctx;
	}
	else {
		return NULL;
	}
}

void
cobweb_handle_retireall() {
	for (size_t i = 0; i < H->slot_size; i++) {
		H->slot[i] = NULL;
		H->name[i].handle = 0;
		memset(H->name[i].name, 0, MAX_NAME_SIZE);
	}
}

uint32_t
cobweb_handle_findname(const char* name) {
	uint32_t handle = 0;

	platform_mutex_lock(H->mutex);

	int n = (int)H->handle_index;
	for (int i = 0; i < n; i++) {
		struct handle_name_t* n = &H->name[i];
		int c = strcmp(n->name, name);
		if (c == 0) {
			handle = n->handle;
			break;
		}
	}

	platform_mutex_unlock(H->mutex);

	return handle;
}

static const char*
_insert_name(struct handle_storage_t* h, const char* name, uint32_t handle) {
	char* result = cobweb_strdup(name);
	h->name[handle].handle = handle;
	strcpy(h->name[handle].name, result);
	return result;
}

const char*
cobweb_handle_namehandle(uint32_t handle, const char* name) {
	platform_mutex_lock(H->mutex);
	const char* ret = _insert_name(H, name, handle);
	platform_mutex_unlock(H->mutex);
	return ret;
}

void
cobweb_handle_init(int harbor) {
	H = (struct handle_storage_t*)cobweb_malloc(sizeof(struct handle_storage_t));
	assert(H != NULL);
	H->slot_size = DEFAULT_SLOT_SIZE;
	H->slot = (struct context_t**)cobweb_malloc(H->slot_size * sizeof(struct context_t*));
	assert(H->slot != NULL);
	memset(H->slot, 0, H->slot_size * sizeof(struct context_t*));
	H->mutex = platform_mutex_create();
	H->harbor = (uint32_t)(harbor & 0xff) << HANDLE_REMOTE_SHIFT;
	/* zero is logger handle */
	H->handle_index = 1;
	H->name = (handle_name_t*)cobweb_malloc(H->slot_size * sizeof(struct handle_name_t));
}

void
cobweb_handle_release() {
	assert(H != NULL);
	cobweb_handle_retireall();
	cobweb_free (H->slot);
	cobweb_free (H->name);
	platform_mutex_release(H->mutex);
	cobweb_free (H);
	H = NULL;
}


/*********************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_module.c
Description: 动态加载so(linux), dll(windows)扩展库,扩展库必须定时4个函数, init, create, release, signal
Author: ydlc
Version: 1.0
Date: 2021.12.11
History:
**********************************************/

#include "cobweb.h"
#include <assert.h>


// 模块类型数量
#define MAX_MODULE_TYPE 32
#define MAX_MODULE_NAME 255

typedef void* (*cobweb_dl_create)(void);
typedef int (*cobweb_dl_init)(void* inst, struct context_t* ctx, const char* parm);
typedef void (*cobweb_dl_release)(void* inst);
typedef void (*cobweb_dl_signal)(void* inst, int signal);

// 模块
struct module_t {
	char name[MAX_MODULE_NAME];//指向模块名
	void* module;//指向dlopen返回的动态库指针
	cobweb_dl_create create;//指向模块内部的xxx_create函数指针，下面3个同理。
	cobweb_dl_init init;
	cobweb_dl_release release;
	cobweb_dl_signal signal;
};

// 模块管理者
struct modules_t {
	int count; // 模块数量
	struct mutex_t* mutex; // 锁
	const char* path; // 模块路径
	struct module_t ms[MAX_MODULE_TYPE]; // 模块
};


static struct modules_t* M = NULL;


static void*
_try_open(const char* name) {
	const char* l;
	const char* path = M->path;
	size_t path_size = strlen(path);
	size_t name_size = strlen(name);

	size_t sz = path_size + name_size;
	void* dl = NULL;
	char* tmp = (char*)cobweb_malloc(sizeof(char) * sz);
	assert(tmp != NULL);
	do
	{
		memset(tmp, 0, sz);
		while (*path == ';')
			path++;
		if (*path == '\0')
			break;
		l = strchr(path, ';');
		if (l == NULL) l = path + strlen(path);
		int len = l - path;
		int i;
		for (i = 0; path[i] != '?' && i < len; i++) {
			tmp[i] = path[i];
		}
		memcpy(tmp + i, name, name_size);
		if (path[i] == '?') {
			strncpy(tmp + i + name_size, path + i + 1, len - i - 1);
		}
		else {
			platform_error("Invalid C service path");
			break;
		}
		if (platform_file_exists(tmp)) {
			dl = platform_dlopen(tmp);
		}
	
		path = l;
	} while (dl == NULL);

	cobweb_free (tmp);

	if (dl == NULL) {
		char msg[512] = { 0 };
		platform_dlerror(msg);
		platform_error("try open %s failed : %s", name, msg);
	}

	return dl;
}

static int
_open_sym(struct module_t* mod) {
	size_t name_size = strlen(mod->name);
	char* tmp = (char*)cobweb_malloc(sizeof(char) * (name_size + 9));
	if (tmp != NULL) {
		memcpy(tmp, mod->name, name_size);
		strcpy(tmp + name_size, "_create");
		mod->create = (cobweb_dl_create)platform_dlsym(mod->module, tmp);
		strcpy(tmp + name_size, "_init");
		mod->init = (cobweb_dl_init)platform_dlsym(mod->module, tmp);
		strcpy(tmp + name_size, "_release");
		mod->release = (cobweb_dl_release)platform_dlsym(mod->module, tmp);
		strcpy(tmp + name_size, "_signal");
		mod->signal = (cobweb_dl_signal)platform_dlsym(mod->module, tmp);
		cobweb_free (tmp);
	}
	return mod->init == NULL;
}

static struct module_t*
_query(const char* name) {
	int i;
	for (i = 0; i < M->count; i++) {
		if (strcmp(M->ms[i].name, name) == 0) {
			return &M->ms[i];
		}
	}
	return NULL;
}


struct module_t*
cobweb_module_query(const char* name) {
	struct module_t* result = _query(name);
	if (result) {
		return result;
	}

	platform_mutex_lock(M->mutex);

	result = _query(name); // double check

	if (result == NULL && M->count < MAX_MODULE_TYPE) {
		int index = M->count;
		void* dl = _try_open(name);
		if (dl) {
			strcpy(M->ms[index].name, name);
			M->ms[index].module = dl;
			if (_open_sym(&M->ms[index]) == 0) {
				M->count++;
				result = &M->ms[index];
			}
			else {
				// 加载函数失败释放
				platform_dlclose(M->ms[index].module);
				memset(M->ms[index].name, 0, MAX_MODULE_NAME);
				M->ms[index].module = NULL;
			}
		}
	}

	platform_mutex_unlock(M->mutex);

	return result;
}

void
cobweb_module_insert(struct module_t* mod) {
	platform_mutex_lock(M->mutex);

	struct module_t* m = _query(mod->name);
	assert(m == NULL && M->count < MAX_MODULE_TYPE);
	int index = M->count;
	M->ms[index] = *mod;
	++M->count;

	platform_mutex_unlock(M->mutex);

}

void
cobweb_module_init(const char* path) {
	M = (struct modules_t*)cobweb_malloc(sizeof(struct modules_t));
	if (M != NULL) {
		memset(M, 0, sizeof(struct modules_t));
		M->mutex = platform_mutex_create();
		M->path = path;
	}
}

void
cobweb_module_release(void) {
	if (M != NULL) {
		platform_mutex_release(M->mutex);
		cobweb_free (M);
		M = NULL;
	}
}

void*
cobweb_module_instance_create(struct module_t* mod) {
	if (mod->create) {
		return mod->create();
	}
	else {
		return (void*)(intptr_t)(~0);
	}
}

int
cobweb_module_instance_init(struct module_t* mod,
	void* inst,
	struct context_t* ctx,
	const char* parm) {
	return mod->init(inst, ctx, parm);
}

void
cobweb_module_instance_release(struct module_t* mod, void* inst) {
	if (mod->release) {
		mod->release(inst);
	}
}

void
cobweb_module_instance_signal(struct module_t* mod, void* inst, int signal) {
	if (mod->signal) {
		mod->signal(inst, signal);
	}
}


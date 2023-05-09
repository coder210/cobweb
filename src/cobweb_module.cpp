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
#include <list>
#include <vector>
#include <string>
#include <mutex>


typedef void* (*cobweb_dl_create)(void);
typedef int (*cobweb_dl_init)(void* inst, struct context_t* ctx, const char* param);
typedef void (*cobweb_dl_release)(void* inst);
typedef void (*cobweb_dl_signal)(void* inst, int signal);

struct module_t {
	std::string name;
	void* module;
	cobweb_dl_create create;
	cobweb_dl_init init;
	cobweb_dl_release release;
	cobweb_dl_signal signal;
};

struct modules_t {
	std::mutex mutex;
	std::string path;
	std::list<std::string> module_paths;
	std::vector<module_t*> ms;
};


static struct modules_t* M = nullptr;


static void*
_try_open(std::string name) {
	void* dl = nullptr;
	auto iter = M->module_paths.begin();
	do
	{
		std::string filename = "";
		for (size_t i = 0; i < iter->size(); i++) {
			if (iter->at(i) == '?') {
				filename += name;
			}
			else {
				filename += iter->at(i);
			}
		}

		if (platform_file_exists(filename.c_str())) {
			dl = platform_dlopen(filename.c_str());
		}

		iter++;
	} while (iter != M->module_paths.end() && dl == nullptr);

	if (dl == nullptr) {
		char msg[512] = { 0 };
		platform_dlerror(msg);
		platform_error("try open %s failed : %s", name.c_str(), msg);
	}

	return dl;
}

static bool
_open_sym(struct module_t* mod) {
	std::string tmp = mod->name + "_create";
	mod->create = (cobweb_dl_create)platform_dlsym(mod->module, tmp.c_str());
	tmp = mod->name + "_init";
	mod->init = (cobweb_dl_init)platform_dlsym(mod->module, tmp.c_str());
	tmp = mod->name + "_release";
	mod->release = (cobweb_dl_release)platform_dlsym(mod->module, tmp.c_str());
	tmp = mod->name + "_signal";
	mod->signal = (cobweb_dl_signal)platform_dlsym(mod->module, tmp.c_str());
	return mod->init != nullptr;
}

static struct module_t*
_query(const char* name) {
	for (size_t i = 0; i < M->ms.size(); i++) {
		if (M->ms.at(i)->name.compare(name) == 0) {
			return M->ms.at(i);
		}
	}
	return nullptr;
}


struct module_t*
cobweb_module_query(const char* name) {
	struct module_t* result = _query(name);
	if (result == nullptr) {
		M->mutex.lock();
		void* dl = _try_open(name);
		if (dl) {
			result = new module_t();
			result->name = name;
			result->module = dl;
			if (_open_sym(result)) {
				M->ms.push_back(result);
			}
			else {
				platform_dlclose(dl);
				delete result;
				result = nullptr;
			}
		}
		M->mutex.unlock();
	}
	return result;
}


void
cobweb_module_init(const char* path) {
	assert(M == nullptr);
	M = new modules_t();
	assert(M != nullptr);
	M->path = path;
	std::string tmp = "";
	for (size_t i = 0; i < M->path.size(); i++)
	{
		if (M->path.at(i) == ';') {
			M->module_paths.push_back(tmp);
			tmp.clear();
		}
		else {
			tmp += M->path.at(i);
		}
	}
	if (!tmp.empty()) {
		M->module_paths.push_back(tmp);
	}
}

void
cobweb_module_release(void) {
	assert(M != nullptr);
	delete M;
	M = nullptr;
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
cobweb_module_instance_init(struct module_t* mod, void* inst,
	struct context_t* ctx, const char* param) {
	return mod->init(inst, ctx, param);
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


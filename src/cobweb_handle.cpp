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
#include <map>
#include <mutex>


struct handle_storage_t {
	std::mutex mutex;
	uint32_t handle_index;
	std::map<uint32_t, struct context_t*> ctx_slot;
	std::map<uint32_t, std::string> name_slot;
};


static struct handle_storage_t* H = nullptr;


uint32_t
cobweb_handle_register(struct context_t* ctx) {
	uint32_t handle = 0;
	H->mutex.lock();
	handle = H->handle_index++;
	H->ctx_slot.insert(std::map<uint32_t, struct context_t*>::value_type(handle, ctx));
	H->mutex.unlock();
	return handle;
}

int
cobweb_handle_retire(uint32_t handle) {
	auto iter = H->ctx_slot.find(handle);
	if (iter == H->ctx_slot.end()) {
		return 1;
	}
	H->mutex.lock();
	H->ctx_slot.erase(handle);
	H->name_slot.erase(handle);
	H->mutex.unlock();
	return 0;
}

struct context_t*
	cobweb_handle_find(uint32_t handle) {
	auto iter = H->ctx_slot.find(handle);
	if (iter != H->ctx_slot.end() && cobweb_context_handle(iter->second) == handle) {
		return iter->second;
	}
	else {
		return nullptr;
	}
}

void
cobweb_handle_retireall() {
	H->mutex.lock();
	H->ctx_slot.clear();
	H->name_slot.clear();
	H->mutex.unlock();
}

uint32_t
cobweb_handle_findname(const char* name) {
	uint32_t handle = 0;

	H->mutex.lock();
	auto iter = H->name_slot.begin();
	while (iter != H->name_slot.end()) {
		if (iter->second.compare(name) == 0) {
			handle = iter->first;
		}
		iter++;
	}
	H->mutex.unlock();

	return handle;
}

const char*
cobweb_handle_namehandle(uint32_t handle, const char* name) {
	const char* ret = NULL;
	H->mutex.lock();
	auto item = H->name_slot.insert(std::map<uint32_t, std::string>::value_type(handle, name));
	if (item.second) {
		ret = item.first->second.c_str();
	}
	H->mutex.unlock();
	return ret;
}

void
cobweb_handle_init(void) {
	assert(H == nullptr);
	H = new handle_storage_t();
	assert(H != nullptr);
	H->handle_index = 1;
}

void
cobweb_handle_release() {
	assert(H != nullptr);
	cobweb_handle_retireall();
	delete H;
	H = nullptr;
}


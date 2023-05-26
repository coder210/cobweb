/***************************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_handle.cpp
Description: 所有的服务在这里通过handle进行模询到
Author: ydlc
Version: 1.0
Date: 2021.10.10
History:
****************************************************/
#include "handle.h"
#include <mutex>
#include <queue>
#include <map>
#include <cassert>


class Handles {
public:
	uint32_t index_;
	std::mutex mutex_;
	std::map<uint32_t, std::string> name_storage_;
	std::map<uint32_t, Context*> ctx_storage_;
	std::queue<uint32_t> handle_storage_;
};

static Handles* H = nullptr;

void
HandleSystem::Init() {
	assert(H == nullptr);
	H = new Handles();
	assert(H != nullptr);
	H->index_ = SYSTEM_HANDLE;
}

void
HandleSystem::Release() {
	assert(H != nullptr);
	H->name_storage_.clear();
	H->ctx_storage_.clear();
	delete H;
	H = nullptr;
}

std::string
HandleSystem::FindName(uint32_t handle) {
	auto iter = H->name_storage_.find(handle);
	if (iter != H->name_storage_.end()) {
		return iter->second;
	}
	else {
		return "";
	}
}

Context* 
HandleSystem::FindContext(uint32_t handle) {
	auto iter = H->ctx_storage_.find(handle);
	if (iter != H->ctx_storage_.end()) {
		return iter->second;
	}
	else {
		return nullptr;
	}
}

uint32_t
HandleSystem::FindHandle(std::string name) {
	uint32_t handle = 0;
	auto iter = H->name_storage_.begin();
	while (iter != H->name_storage_.end()) {
		if (iter->second == name) {
			handle = iter->first;
			break;
		}
		iter++;
	}
	return handle;
}

uint32_t
HandleSystem::Register(Context* ctx) {
	uint32_t handle = 0;
	H->mutex_.lock();
	H->index_++;
	auto item = H->ctx_storage_.insert(std::map<uint32_t, Context*>::value_type(H->index_, ctx));
	handle = item.first->first;
	H->mutex_.unlock();
	return handle;
}

std::string
HandleSystem::Naming(uint32_t handle, std::string name) {
	std::string ret;
	H->mutex_.lock();
	auto item = H->name_storage_.insert(std::map<uint32_t, std::string>::value_type(handle, name));
	if (item.second) {
		ret = item.first->second;
	}
	H->mutex_.unlock();
	return ret;
}

void
HandleSystem::Retire(uint32_t handle) {
	H->mutex_.lock();
	H->name_storage_.erase(handle);
	H->ctx_storage_.erase(handle);
	H->mutex_.unlock();
}

uint32_t
HandleSystem::Dequeue() {
	uint32_t handle = 0;
	if (!H->handle_storage_.empty()) {
		H->mutex_.lock();
		if (!H->handle_storage_.empty()) {
			handle = H->handle_storage_.front();
			H->handle_storage_.pop();
		}
		H->mutex_.unlock();
	}
	return handle;
}

void
HandleSystem::Enqueue(uint32_t handle) {
	H->mutex_.lock();
	H->handle_storage_.push(handle);
	H->mutex_.unlock();
}

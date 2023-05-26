/***********************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_timer.cpp
Description: 通过时间轮算法实现的定时器
Author: ydlc
Version: 1.0
Date: 2021.5.22
History:
************************************************/
#include "timer.h"
#include <cassert>
#include "platform.h"
#include "handle.h"


#define BACKET_NUM 9


struct Trigger {
	uint32_t handle_;
	int type_;
	int session_;
	void* data_;
	size_t sz_;
	int count_;
};

struct Timer {
	std::mutex mutex_;
	int index_;
	std::vector<std::list<Trigger>*> buckets_;
};

static Timer* T = nullptr;

void
TimerSystem::Init() {
	assert(T == nullptr);
	T = new Timer();
	assert(T != nullptr);
	T->index_ = 0;
	for (size_t i = 0; i < BACKET_NUM; i++) {
		T->buckets_.push_back(new std::list<Trigger>());
	}
}

void
TimerSystem::Release() {
	assert(T != nullptr);
	T->index_ = 0;
	for (size_t i = 0; i < T->buckets_.size(); i++) {
		T->buckets_[i]->clear();
		delete T->buckets_[i];
		T->buckets_[i] = nullptr;
	}
	T->buckets_.clear();
}

int
TimerSystem::Timeout(uint32_t handle, int type, int session, const void* data, size_t sz, size_t time) {
	Context* ctx = HandleSystem::FindContext(handle);
	if (ctx == nullptr) {
		return session;
	}

	if ((type & PTYPE_TAG_STRING) != 0) {
		char* tmp = (char*)malloc(sz + 1);
		if (tmp == nullptr) {
			return session;
		}
		memcpy_s(tmp, sz, data, sz);
		tmp[sz] = 0;
		data = tmp;
	}
	else if ((type & PTYPE_TAG_USERDATA) != 0) {
		void* tmp = malloc(sz);
		if (tmp == nullptr) {
			return session;
		}
		memcpy_s(tmp, sz, data, sz);
		data = tmp;
	}

	Trigger trigger = { 0 };
	trigger.handle_ = handle;
	trigger.type_ = type & 0xff;
	trigger.session_ = session;
	trigger.data_ = (void*)data;
	trigger.sz_ = sz;
	trigger.count_ = time / T->buckets_.size();
	T->mutex_.lock();
	int index = (time + T->index_) % T->buckets_.size();
	T->buckets_[index]->push_back(trigger);
	T->mutex_.unlock();
	return trigger.session_;
}

void
TimerSystem::Tick() {
	int index = 0;
	T->mutex_.lock();
	T->index_++;
	T->index_ %= T->buckets_.size();
	index = T->index_;
	T->mutex_.unlock();
	std::list<Trigger>* triggers = T->buckets_[index];
	int count_ = triggers->size();
	std::list<Trigger>::iterator iter;
	int i = 0;
	for (iter = triggers->begin(); iter != triggers->end() && i < count_; i++) {
		if (iter->count_ == 0) {
			Context* ctx = HandleSystem::FindContext(iter->handle_);
			if (ctx != nullptr) {
				ContextSystem::Push(ctx, SYSTEM_HANDLE, iter->type_, iter->session_, iter->data_, iter->sz_);
			}
			iter->count_--;
			iter++;
		}
		else if (iter->count_ > 0) {
			iter->count_--;
			iter++;
		}
		else {
			T->mutex_.lock();
			iter = triggers->erase(iter);
			T->mutex_.unlock();
		}
	}
}

void
TimerSystem::Sleep(void) {
	// std::this_thread::sleep_for(std::chrono::milliseconds(10));
	PlatformSystem::msleep(10);
}



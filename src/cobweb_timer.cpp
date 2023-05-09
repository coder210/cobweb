/***********************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_timer.cpp
Description: 通过时间轮算法实现的定时器
Author: ydlc
Version: 1.0
Date: 2021.5.22
History:
************************************************/
#include "cobweb.h"
#include <vector>
#include <list>
#include <mutex>


#define TRIGGER_NUM 9


struct trigger_t {
	uint32_t handle;
	int session;
	int count;
	void* data;
	size_t sz;
};

struct timer_t {
	std::mutex mutex;
	int cur_index;
	std::vector<std::list<struct trigger_t>*> bucket;
};


static struct timer_t* T = nullptr;


void
cobweb_timer_init(void) {
	assert(T == nullptr);
	T = new timer_t();
	assert(T != nullptr);
	T->cur_index = 0;
	for (size_t i = 0; i < TRIGGER_NUM; i++) {
		T->bucket.push_back(new std::list<struct trigger_t>());
	}
}

int
cobweb_add_trigger(uint32_t handle, int session, int time, void* data, size_t sz) {
	struct trigger_t trigger;
	trigger.handle = handle;
	trigger.session = session;
	trigger.data = data;
	trigger.sz = sz;
	trigger.count = time / T->bucket.size();
	T->mutex.lock();
	int index = (time + T->cur_index) % T->bucket.size();
	T->bucket[index]->push_back(trigger);
	T->mutex.unlock();
	return session;
}


int
cobweb_timer_tick(void*) {
	T->cur_index++;
	T->cur_index %= T->bucket.size();
	std::list<struct trigger_t>* trigger_list = T->bucket[T->cur_index];
	auto iter = trigger_list->begin();
	while (iter != trigger_list->end()) {
		if (iter->count <= 0) {
			message_t msg;
			msg.data = iter->data;
			msg.session = iter->session;
			msg.sz = iter->sz;
			msg.source = 0;
			msg.type = PTYPE_RESPONSE;
			cobweb_context_push(iter->handle, msg);
			T->mutex.lock();
			iter = trigger_list->erase(iter);
			T->mutex.unlock();
		}
		else {
			iter->count--;
			iter++;
		}
	}
	return T->cur_index;
}

void
cobweb_timer_sleep(void) {
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void
cobweb_timer_release(void) {
	assert(T != nullptr);
	for (size_t i = 0; i < T->bucket.size(); i++) {
		T->bucket[i]->clear();
		delete T->bucket[i];
		T->bucket[i] = nullptr;
	}
	T->bucket.clear();
	delete T;
	T = nullptr;
}


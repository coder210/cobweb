/*********************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_cq.cpp
Description: 消息队列
Author: ydlc
Version: 1.0
Date: 2021.4.22
History:
***********************************************/
#include "cobweb.h"
#include <string>
#include <mutex>
#include <queue>


struct cq_t {
	std::mutex mutex;
	std::queue<struct context_t*> queue;
};


static struct cq_t* Q = nullptr;


void
cobweb_cq_init(void) {
	assert(Q == nullptr);
	Q = new cq_t();
	assert(Q != nullptr);
}

void
cobweb_cq_eq(struct context_t* ctx) {
	Q->mutex.lock();
	if (ctx != nullptr) {
		Q->queue.push(ctx);
	}
	Q->mutex.unlock();
}

struct context_t*
cobweb_cq_dq() {
	struct context_t* ctx = nullptr;
	if (Q->queue.empty()) {
		return ctx;
	}
	if (Q->mutex.try_lock()) {
		if (!Q->queue.empty()) {
			ctx = Q->queue.front();
			Q->queue.pop();
		}
		Q->mutex.unlock();
	}
	return ctx;
}

void
cobweb_cq_release() {
	assert(Q != nullptr);
	delete Q;
	Q = nullptr;
}

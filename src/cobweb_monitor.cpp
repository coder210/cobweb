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
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>


struct monitor_t {
	int version;
	int check_version;
	uint32_t source;
	uint32_t dest;
};


struct monitors_t {
	std::vector<struct monitor_t*> ms;
	std::mutex mutex;
	std::condition_variable cv;
	bool quit;
};


static struct monitors_t* M = nullptr;


void
cobweb_monitor_init(void) {
	assert(M == nullptr);
	M = new monitors_t();
	assert(M != nullptr);
	M->quit = false;
}

void
cobweb_monitor_release(void) {
	assert(M != nullptr);
	for (size_t i = 0; i < M->ms.size(); i++) {
		delete M->ms[i];
	}
	M->ms.clear();
	delete M;
	M = nullptr;
}

struct monitor_t*
	cobweb_monitor_new() {
	struct monitor_t* m = new monitor_t();
	memset(m, 0, sizeof(struct monitor_t));
	M->mutex.lock();
	M->ms.push_back(m);
	M->mutex.unlock();
	return m;
}

void
cobweb_monitor_trigger(struct monitor_t* m, uint32_t source, uint32_t dest) {
	m->source = source;
	m->dest = dest;
	m->version++;
}

void
cobweb_monitor_check() {
	for (size_t i = 0; i < M->ms.size(); i++) {
		struct monitor_t* m = M->ms[i];
		if (m->version == m->check_version) {
			if (m->dest) {
				struct context_t* ctx = cobweb_handle_find(m->dest);
				cobweb_context_error(ctx, "A message from [ :%08x ] to [ :%08x ] maybe in an endless loop (version = %d)", m->source, m->dest, m->version);
			}
		}
		else {
			m->check_version = m->version;
		}
	}
}

void
cobweb_monitor_sleep(void) {
	for (size_t i = 0; i < M->ms.size(); i++) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

bool
cobweb_monitor_quit(void) {
	return M->quit;
}

void
cobweb_monitor_wait(void) {
	std::unique_lock <std::mutex> lck(M->mutex);
	platform_log("阻塞中");
	M->cv.wait(lck); 
	platform_log("解除阻塞");
}

void
cobweb_monitor_wakeup(void) {
	std::unique_lock <std::mutex> lck(M->mutex);
	M->cv.notify_one();
}

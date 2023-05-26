/********************************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: CobwebSystem_monitor.cpp
Description: 监控消息分发时的状态,出现异常进行预警
Author: ydlc
Version: 1.0
Date: 2021.12.10
History:
********************************************************/

#include "monitor.h"
#include <vector>
#include <mutex>
#include <cassert>
#include <cstring>
#include <condition_variable>  
#include "platform.h"


class Monitors {
public:
	std::vector<Monitor*> ms_;
	std::mutex mutex_;
	std::condition_variable cv_;
	bool quit_;
};


static Monitors* M = nullptr;


void
MonitorSystem::Init(int thread) {
	assert(M == nullptr);
	M = new Monitors();
	assert(M != nullptr);
	M->mutex_.lock();
	for (int i = 0; i < thread; i++) {
		Monitor* m = new Monitor();
		memset(m, 0, sizeof(Monitor));
		M->ms_.push_back(m);
	}
	M->quit_ = false;
	M->mutex_.unlock();
}

void
MonitorSystem::Release(void) {
	assert(M != nullptr);
	for (size_t i = 0; i < M->ms_.size(); i++) {
		delete M->ms_[i];
	}
	M->ms_.clear();
	delete M;
	M = nullptr;
}

void
MonitorSystem::Sleep(void) {
	for (size_t i = 0; i < M->ms_.size(); i++) {
		// std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		PlatformSystem::msleep(1000);
	}
}

void
MonitorSystem::Inspect(void(*error_msg)(std::string, void*), void* ptr) {
	for (size_t i = 0; i < M->ms_.size(); i++) {
		Monitor* monitor = M->ms_.at(i);
		if (monitor->version_ == monitor->check_version_) {
			if (monitor->dest_) {
				char str[128] = { 0 };
				std::sprintf(str, "A message from [ :%08x ] to [ :%08x ] maybe in an endless loop (version = %d)", monitor->source_, monitor->dest_, monitor->version_);
				error_msg(str, ptr);
			}
		}
		else {
			monitor->check_version_ = monitor->version_;
		}
	}
}

bool
MonitorSystem::get_quit(void) {
	if (M->quit_) {
		M->cv_.notify_one();
	}
	return M->quit_;
}

bool
MonitorSystem::is_continue() {
	return !M->quit_;
}

void
MonitorSystem::Quit(void) {
	M->quit_ = true;
}

void
MonitorSystem::Wait(void) {
	auto next_time = std::chrono::system_clock::now() + std::chrono::seconds(1);
	std::unique_lock <std::mutex> lck(M->mutex_);
	M->cv_.wait_until(lck, next_time);
}

void
MonitorSystem::Wakeup(void) {
	 std::unique_lock <std::mutex> lck(M->mutex_);
	 M->cv_.notify_one();
}

void
MonitorSystem::Trigger(int id, uint32_t source, uint32_t dest) {
	Monitor* monitor = M->ms_.at(id);
	monitor->source_ = source;
	monitor->dest_ = dest;
	monitor->version_++;
}

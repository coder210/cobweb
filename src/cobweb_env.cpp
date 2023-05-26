/************************************************
Copyright: 2021-2022, lanchong.xyz/Ltd.
File name: cobweb_env.cpp
Description: 存储变量的值
Author: ydlc
Version: 1.0
Date: 2021.12.14
History:
*************************************************/
#include "env.h"
#include <mutex>
#include <map>
#include <cassert>

class Env {
public:
	std::mutex mutex_;
	std::map<std::string, std::string> storage_;
};


static Env* E = nullptr;


void
EnvSystem::Init() {
	assert(E == nullptr);
	E = new Env();
	assert(E != nullptr);
}

void
EnvSystem::Release() {
	assert(E != nullptr);
	delete E;
	E = nullptr;
}

std::string
EnvSystem::Get(std::string key) {
	std::string str;
	E->mutex_.lock();
	auto iter = E->storage_.find(key);
	if (iter != E->storage_.end()) {
		str = iter->second;
	}
	E->mutex_.unlock();
	return str;
}

void
EnvSystem::Set(std::string key, std::string value) {
	E->mutex_.lock();
	auto iter = E->storage_.find(key);
	if (iter == E->storage_.end()) {
		E->storage_.insert(std::map<std::string, std::string>::value_type(key, value));
	}
	E->mutex_.unlock();
}


/************************************************
Copyright: 2021-2022, lanchong.xyz/Ltd.
File name: cobweb_env.cpp
Description: 存储变量的值
Author: ydlc
Version: 1.0
Date: 2021.12.14
History:
*************************************************/

#include "cobweb.h"
#include <string>
#include <mutex>
#include <map>

struct env_t {
	std::mutex mutex;
	std::map<std::string, std::string> storage;
};


static struct env_t* E = nullptr;


void
cobweb_env_init(void) {
	assert(E == nullptr);
	E = new env_t();
}

const char*
cobweb_getenv(const char* key) {
	const char* result = NULL;
	E->mutex.lock();
	auto iter = E->storage.find(key);
	if (iter != E->storage.end()) {
		result = iter->second.c_str();
	}
	E->mutex.unlock();
	return result;
}

void
cobweb_setenv(const char* key, const char* value) {
	E->mutex.lock();
	auto iter = E->storage.find(key);
	if (iter == E->storage.end()) {
		E->storage.insert(std::map<std::string, std::string>::value_type(key, value));
	}
	E->mutex.unlock();
}

void
cobweb_env_release(void) {
	assert(E != nullptr);
	delete E;
	E = nullptr;
}


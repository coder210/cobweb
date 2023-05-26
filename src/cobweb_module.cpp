/*********************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_module.cpp
Description: 动态加载so(linux), dll(windows)扩展库,扩展库必须定时4个函数, init, create, release, signal
Author: ydlc
Version: 1.0
Date: 2021.12.11
History:
**********************************************/

#include "module.h"
#include "platform.h"
#include "utils.h"
#include <map>

class Modules {
public:
	std::mutex mutex_;
	std::vector<std::string> module_paths_;
	std::vector<Module*> module_pool_;
};


static Modules* M = nullptr;

void
ModuleSystem::Init(std::string path) {
	assert(M == nullptr);
	M = new Modules();
	assert(M != nullptr);
	UtilsSystem::SplitString(path, ';', M->module_paths_);
}

void
ModuleSystem::Release(void) {
	assert(M != nullptr);
	for (size_t i = 0; i < M->module_pool_.size(); i++) {
		PlatformSystem::dllclose(M->module_pool_[i]);
		delete M->module_pool_[i];
	}
	M->module_pool_.clear();
	delete M;
	M = nullptr;
}

Module*
ModuleSystem::Query(std::string name) {
	Module* result = nullptr;
	for (size_t i = 0; i < M->module_pool_.size(); i++) {
		if (M->module_pool_.at(i)->name_ == name) {
			result = M->module_pool_.at(i);
			break;
		}
	}

	if (result != nullptr) {
		return result;
	}

	void* dl = ModuleSystem::Open(name);
	if (dl == nullptr) {
		char msg[512] = { 0 };
		PlatformSystem::dllerror(msg);
		PlatformSystem::error("try open %s failed : %s", name.c_str(), msg);
		return result;
	}

	M->mutex_.lock();
	Module* module = new Module();
	module->name_ = name;
	module->dl_ = dl;
	std::string tmp = module->name_ + "_create";
	module->create_ = (dl_create)PlatformSystem::dllsym(module->dl_, tmp.c_str());
	tmp = module->name_ + "_init";
	module->init_ = (dl_init)PlatformSystem::dllsym(module->dl_, tmp.c_str());
	tmp = module->name_ + "_release";
	module->release_ = (dl_release)PlatformSystem::dllsym(module->dl_, tmp.c_str());
	tmp = module->name_ + "_signal";
	module->signal_ = (dl_signal)PlatformSystem::dllsym(module->dl_, tmp.c_str());
	M->mutex_.unlock();

	if (module->init_ != nullptr) {
		result = module;
	}
	else {
		PlatformSystem::error("call init function failed!");
		PlatformSystem::dllclose(module->dl_);
		delete module;
	}

	return result;
}

void*
ModuleSystem::Open(std::string name) {
	void* dl = nullptr;
	auto iter = M->module_paths_.begin();
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
		if (UtilsSystem::FileExists(filename.c_str())) {
			dl = PlatformSystem::dllopen(filename.c_str());
		}
		iter++;
	} while (iter != M->module_paths_.end() && dl == nullptr);
	return dl;
}

void*
ModuleSystem::InstanceCreate(Module* module) {
	if (module->create_) {
		return module->create_();
	}
	else {
		return (void*)(intptr_t)(~0);
	}
}

bool
ModuleSystem::InstanceInit(Module* module, void* inst, void* ctx, std::string param) {
	return module->init_(inst, ctx, param);
}

void
ModuleSystem::InstanceRelease(Module* module, void* inst) {
	if (module->release_) {
		module->release_(inst);
	}
}

void
ModuleSystem::InstanceSignal(Module* module, void* inst, int signal) {
	if (module->signal_) {
		module->signal_(inst, signal);
	}
}


/************************************************
Copyright: 2021-2022, lanchong.xyz/Ltd.
File name: cobweb_start.cpp
Description: 开始启动服务器
Author: ydlc
Version: 1.0
Date: 2021.12.14
History:
*************************************************/
#include "cobweb.h"
#include <thread>
#include <chrono>
#include <vector>
#include "platform.h"
#include "timer.h"
#include "module.h"
#include "monitor.h"
#include "handle.h"
#include "env.h"

void
CobwebSystem::PrintBlessing() {
	ContextSystem::Log(SYSTEM_HANDLE, "                 _ooOoo_  ");
	ContextSystem::Log(SYSTEM_HANDLE, "                o8888888o  ");
	ContextSystem::Log(SYSTEM_HANDLE, "                88\" . \"88    ");
	ContextSystem::Log(SYSTEM_HANDLE, "                (| -_- |)    ");
	ContextSystem::Log(SYSTEM_HANDLE, "             ____/`---'\\____   ");
	ContextSystem::Log(SYSTEM_HANDLE, "           .'  \\|     |//  `.    ");
	ContextSystem::Log(SYSTEM_HANDLE, "           /  \\|||  :  |||//  \\    ");
	ContextSystem::Log(SYSTEM_HANDLE, "          /  _||||| -:- |||||-  \\    ");
	ContextSystem::Log(SYSTEM_HANDLE, "          |   | \\\\  -  /// |   |    ");
	ContextSystem::Log(SYSTEM_HANDLE, "         | \\_|  ''\\---/''  |   |   ");
	ContextSystem::Log(SYSTEM_HANDLE, "          \\  .-\\__  `-`  ___/-. /    ");
	ContextSystem::Log(SYSTEM_HANDLE, "         ___`. .'  /--.--\\  `. . __    ");
	ContextSystem::Log(SYSTEM_HANDLE, "      .\"\" '<  `.___\\_<|>_/___.'  >'\"\".    ");
	ContextSystem::Log(SYSTEM_HANDLE, "     | | :  `- \\`.;`\\ _ /`;.`/ - ` : | |    ");
	ContextSystem::Log(SYSTEM_HANDLE, "   \\  \\ `-.   \\_ __\\ /__ _/   .-` /  /    ");
	ContextSystem::Log(SYSTEM_HANDLE, "  =====`-.____`-.___\\_____/___.-`____.-'======    ");
	ContextSystem::Log(SYSTEM_HANDLE, " `^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^      ");
	ContextSystem::Log(SYSTEM_HANDLE, " `          佛祖保佑       永无Bug     ");
	ContextSystem::Log(SYSTEM_HANDLE, " `          快加工资       不改需求      ");
}

int
CobwebSystem::OptInt(std::string key, int opt) {
	std::string str = EnvSystem::Get(key);
	if (str == "") {
		char tmp[20];
		sprintf(tmp, "%d", opt);
		EnvSystem::Set(key, tmp);
		return opt;
	}
	return std::stoi(str);
}

std::string
CobwebSystem::OptString(std::string key, std::string opt) {
	std::string str = EnvSystem::Get(key);
	if (str.empty()) {
		if (!opt.empty()) {
			EnvSystem::Set(key, opt);
			str = EnvSystem::Get(opt);
		}
	}
	return str;
}

void
CobwebSystem::InitEnv(lua_State* L) {
	lua_pushnil(L);
	while (lua_next(L, -2) != 0) {
		int keyt = lua_type(L, -2);
		if (keyt != LUA_TSTRING) {
			PlatformSystem::info("Invalid config table");
			return;
		}
		const char* key = lua_tostring(L, -2);
		if (lua_type(L, -1) == LUA_TBOOLEAN) {
			int b = lua_toboolean(L, -1);
			EnvSystem::Set(key, b ? "true" : "false");
		}
		else {
			const char* value = lua_tostring(L, -1);
			if (value == NULL) {
				PlatformSystem::error("Invalid config table key = %s", key);
				return;
			}
			EnvSystem::Set(key, value);
		}
		lua_pop(L, 1);
	}
	lua_pop(L, 1);
}

void
CobwebSystem::MonitorError(std::string msg, void* ptr) {
	ContextSystem::LogError(SYSTEM_HANDLE, msg.c_str());
}

void
CobwebSystem::ThreadMonitor(void* ptr) {
	while (!MonitorSystem::get_quit()) {
		MonitorSystem::Inspect(CobwebSystem::MonitorError, ptr);
		MonitorSystem::Sleep();
	}
}

void
CobwebSystem::ThreadTimer(void* ptr) {
	while (!MonitorSystem::get_quit()) {
		TimerSystem::Tick();
		TimerSystem::Sleep();
	}
}

void
CobwebSystem::ThreadMain(void* ptr) {
	std::string project = EnvSystem::Get("project");
	if (ContextSystem::New("logger", project) == nullptr) {
		PlatformSystem::error("logger service lunch fail...");
		MonitorSystem::Quit();
		return;
	}

	CobwebSystem::PrintBlessing();

	std::string bootstrap = EnvSystem::Get("bootstrap");
	if (ContextSystem::New("luax", bootstrap) == nullptr) {
		ContextSystem::LogError(SYSTEM_HANDLE, "%s service lunch fail...", bootstrap.c_str());
	}
}

void
CobwebSystem::ThreadWorker(void* ptr) {
	int mid = (int)ptr;
	while (!MonitorSystem::get_quit()) {
		uint32_t handle = HandleSystem::Dequeue();
		if (handle != 0) {
			Context* ctx = HandleSystem::FindContext(handle);
			if (ctx != nullptr) {
				ContextSystem::DispatchMessage(ctx, mid);
				HandleSystem::Enqueue(handle);
			}
			else {
				ContextSystem::Log(handle, "EXIT");
			}
		}
		else {
			MonitorSystem::Wait();
		}
	}
}

int
CobwebSystem::Start(const char* config_file) {
	struct lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	int err = luaL_dofile(L, config_file);
	if (err) {
		lua_close(L);
		return 1;
	}

	EnvSystem::Init();

	CobwebSystem::InitEnv(L);

	Config config;
	config.thread_ = CobwebSystem::OptInt("thread", 1);
	config.module_path_ = CobwebSystem::OptString("cpath", "./cservice/?.so");
	config.bootstrap_ = CobwebSystem::OptString("bootstrap", "luax bootstrap");
	config.logservice_ = CobwebSystem::OptString("logservice", "logger");
	config.project_ = CobwebSystem::OptString("project", "");

	lua_close(L);

	HandleSystem::Init();
	ModuleSystem::Init(config.module_path_);
	MonitorSystem::Init(config.thread_);
	TimerSystem::Init();

	std::vector<std::thread> thread_vector;
	thread_vector.push_back(std::thread(CobwebSystem::ThreadMonitor, nullptr));
	thread_vector.push_back(std::thread(CobwebSystem::ThreadTimer, nullptr));
	thread_vector.push_back(std::thread(CobwebSystem::ThreadMain, nullptr));

	for (int i = 0; i < config.thread_; i++) {
		thread_vector.push_back(std::thread(CobwebSystem::ThreadWorker, (void*)i));
	}
	for (size_t i = 0; i < thread_vector.size(); i++) {
		thread_vector.at(i).join();
	}

	TimerSystem::Release();
	MonitorSystem::Release();
	ModuleSystem::Release();
	HandleSystem::Release();
	EnvSystem::Release();

	return 0;
}




/**************************************************
Copyright: 2021-2022, lanchong.xyz/Ltd.
File name: cobweb_server.cpp
Description: 此服务器采用ctx模型,也是主要操作对象
Author: ydlc
Version: 1.0
Date: 2021.12.12
History:
****************************************************/
#include <map>
#include <mutex>
#include <cassert>
#include <cstring>
#include "context.h"
#include "handle.h"
#include "monitor.h"
#include "platform.h"
#include "utils.h"
#include "env.h"
#include "timer.h"


#define MAX_BUFFER 512;


using command_func = std::string(*)(Context*, std::string);


struct ContextData {
	void* instance_;
	void* cb_ud_;
	Module* mod_;
	MQ mq_;
	context_callback cb_;
	FILE* logfile_;
	std::string result_;
	uint32_t handle_;
	int session_;
	bool endless_;
	bool dead_;
	std::map<std::string, command_func> funcs_;
};

std::string
ContextSystem::Version() {
	return std::string("v1.0.0");
}

uint32_t
ContextSystem::get_handle(Context* ctx) {
	return ctx->data_->handle_;
}

Context*
ContextSystem::New(std::string name, std::string param) {
	Module* mod = ModuleSystem::Query(name);
	if (mod == nullptr) {
		return nullptr;
	}
	void* inst = ModuleSystem::InstanceCreate(mod);
	if (inst == nullptr) {
		return nullptr;
	}

	Context* ctx = new Context();
	ctx->data_ = new ContextData();
	ctx->data_->handle_ = HandleSystem::Register(ctx);
	ctx->data_->instance_ = inst;
	ctx->data_->mod_ = mod;
	ctx->data_->result_ = "";
	ctx->data_->logfile_ = nullptr;
	ctx->data_->endless_ = false;
	ctx->data_->session_ = 0;
	ctx->data_->cb_ = nullptr;
	ctx->data_->cb_ud_ = nullptr;
	ctx->data_->dead_ = false;

	ctx->data_->funcs_.insert({ "REG", ContextSystem::CMD_REG });
	ctx->data_->funcs_.insert({ "QUERY", ContextSystem::CMD_QUERY });
	ctx->data_->funcs_.insert({ "NAME", ContextSystem::CMD_NAME });
	ctx->data_->funcs_.insert({ "GETENV", ContextSystem::CMD_GETENV });
	ctx->data_->funcs_.insert({ "EXIT", ContextSystem::CMD_EXIT });
	ctx->data_->funcs_.insert({ "KILL", ContextSystem::CMD_KILL });
	ctx->data_->funcs_.insert({ "LAUNCH", ContextSystem::CMD_LAUNCH });
	ctx->data_->funcs_.insert({ "TIMEOUT", ContextSystem::CMD_TIMEOUT });

	ctx->callback = ContextSystem::Callback;
	ctx->command = ContextSystem::Command;
	ctx->info = PlatformSystem::info;
	ctx->debug = PlatformSystem::debug;
	ctx->error = PlatformSystem::error;
	ctx->log = ContextSystem::Log;
	ctx->log_error = ContextSystem::LogError;
	ctx->timestamp = UtilsSystem::Timestamp;
	ctx->current_datetime = UtilsSystem::CurrentDatetime;
	ctx->timeout = TimerSystem::Timeout;
	ctx->get_handle = ContextSystem::get_handle;
	ctx->push = ContextSystem::Push;
	ctx->send = ContextSystem::Send;
	ctx->sendname = ContextSystem::Sendname;
	ctx->newsession = ContextSystem::NewSession;

	if (ModuleSystem::InstanceInit(mod, inst, ctx, param)) {
		HandleSystem::Enqueue(ctx->data_->handle_);
	}
	else {
		ContextSystem::Release(ctx);
		ctx = nullptr;
	}

	return ctx;
}

void
ContextSystem::Release(Context* ctx) {
	assert(ctx->data_ != nullptr);
	assert(ctx != nullptr);
	ModuleSystem::InstanceRelease(ctx->data_->mod_, ctx->data_->instance_);
	HandleSystem::Retire(ctx->data_->handle_);
	delete ctx->data_;
	delete ctx;
	ctx = nullptr;
}

void
ContextSystem::Log(uint32_t source, const char* format, ...) {
	uint32_t logger = HandleSystem::FindHandle(".logger");
	Context* ctx = HandleSystem::FindContext(logger);
	if (ctx == nullptr) {
		return;
	}
	std::string str;
	va_list ap;
	va_start(ap, format);
	int length = _vscprintf(format, ap) + 1;
	std::vector<char> char_vector(length);
	_vsnprintf(char_vector.data(), length, format, ap);
	str.assign(char_vector.data());
	va_end(ap);
	ContextSystem::Push(ctx, source, PTYPE_TEXT | PTYPE_TAG_STRING, 0, str.c_str(), str.size());
}

void
ContextSystem::LogError(uint32_t source, const char* format, ...) {
	uint32_t logger = HandleSystem::FindHandle(".logger");
	Context* ctx = HandleSystem::FindContext(logger);
	if (ctx == nullptr) {
		return;
	}
	std::string str;
	va_list ap;
	va_start(ap, format);
	int length = _vscprintf(format, ap) + 1;
	std::vector<char> char_vector(length);
	_vsnprintf(char_vector.data(), length, format, ap);
	str.assign(char_vector.data());
	va_end(ap);
	ContextSystem::Push(ctx, source, PTYPE_ERROR | PTYPE_TAG_STRING, 0, str.c_str(), str.size());
}

void
ContextSystem::DispatchMessage(Context* ctx, int mid) {
	ContextData* ctx_data = ctx->data_;
	if (ctx_data->dead_) {
		ContextSystem::Release(ctx);
		return;
	}
	if (ctx_data->cb_ == nullptr) {
		return;
	}
	Message msg;
	if (!MQSystem::Dequeue(&ctx_data->mq_, &msg)) {
		return;
	}
	MonitorSystem::Trigger(mid, msg.source_, ctx_data->handle_);
	if (ctx_data->cb_ != nullptr) {
		if (ctx_data->logfile_) {

		}
		if (!ctx_data->cb_(ctx, ctx_data->cb_ud_, msg.type_, msg.session_, msg.source_, msg.data_, msg.sz_)) {
			free(msg.data_);
		}
	}
	MonitorSystem::Trigger(mid, 0, 0);
}

bool
ContextSystem::Push(Context* ctx, uint32_t source, int type, int session, const void* data, size_t sz) {
	if ((type & PTYPE_TAG_STRING) != 0) {
		char* tmp = (char*)malloc(sz + 1);
		if (tmp == nullptr) {
			return false;
		}
		memcpy_s(tmp, sz, data, sz);
		tmp[sz] = 0;
		data = tmp;
	}
	else if ((type & PTYPE_TAG_USERDATA) != 0) {
		void* tmp = malloc(sz);
		if (tmp == nullptr) {
			return false;
		}
		memcpy_s(tmp, sz, data, sz);
		data = tmp;
	}
	Message msg;
	msg.source_ = source;
	msg.type_ = type & 0xff;
	msg.session_ = session;
	msg.data_ = (void*)data;
	msg.sz_ = sz;
	MQSystem::Enqueue(&ctx->data_->mq_, msg);
	MonitorSystem::Wakeup();
	return true;
}

bool
ContextSystem::Send(uint32_t source, uint32_t dest, int type, int session, const void* data, size_t sz) {
	Context* source_ctx = HandleSystem::FindContext(source);
	if (source_ctx == nullptr) {
		return false;
	}
	Context* dest_ctx = HandleSystem::FindContext(dest);
	if (dest_ctx == nullptr) {
		return false;
	}
	bool ret = ContextSystem::Push(dest_ctx, source, type, session, data, sz);
	return ret;
}

bool
ContextSystem::Sendname(uint32_t source, std::string dest_name, int type, int session, const void* data, size_t sz) {
	Context* source_ctx = HandleSystem::FindContext(source);
	if (source_ctx == nullptr) {
		return false;
	}
	uint32_t dest = HandleSystem::FindHandle(dest_name);
	if (dest == 0) {
		return false;
	}
	Context* dest_ctx = HandleSystem::FindContext(dest);
	if (dest_ctx == nullptr) {
		return false;
	}
	bool ret = ContextSystem::Push(dest_ctx, source, type, session, data, sz);
	return ret;
}

void
ContextSystem::Callback(Context* ctx, void* ud, context_callback cb) {
	ctx->data_->cb_ud_ = ud;
	ctx->data_->cb_ = cb;
}

std::string
ContextSystem::Command(Context* ctx, std::string cmd, std::string param) {
	auto iter = ctx->data_->funcs_.find(cmd);
	if (iter != ctx->data_->funcs_.end()) {
		ctx->data_->result_ = iter->second(ctx, param);
		return ctx->data_->result_;
	}
	else {
		return "0";
	}
}

int
ContextSystem::NewSession(Context* ctx) {
	ctx->data_->session_++;
	return ctx->data_->session_;
}

std::string
ContextSystem::CMD_REG(Context* ctx, std::string name) {
	if (name.at(0) == '.') {
		HandleSystem::Naming(ctx->data_->handle_, name);
	}
	else if (name.at(0) == ':') {
		if (HandleSystem::FindHandle(name) != 0) {
			HandleSystem::Naming(ctx->data_->handle_, name);
		}
	}
	else {

	}
	return "";
}

std::string 
ContextSystem::CMD_QUERY(Context* ctx, std::string param) {
	std::string result = "";
	if (param.at(0) == '.') {
		uint32_t handle = HandleSystem::FindHandle(param);
		if (handle != 0) {
			result = UtilsSystem::FormatString("%ud", handle);
		}
	}
	else if (param.at(0) == ':') {
	
	}
	else {

	}
	return result;
}

std::string 
ContextSystem::CMD_NAME(Context* ctx, std::string param) {
	std::vector<std::string> substr_vecotr;
	UtilsSystem::SplitString(param, ':', substr_vecotr);
	if (substr_vecotr.size() == 2) {
		std::string name = substr_vecotr.at(0);
		uint32_t handle = std::stoi(substr_vecotr.at(1));
		HandleSystem::Naming(handle, name);
		return "1";
	}
	else {
		return "0";
	}
}

std::string
ContextSystem::CMD_GETENV(Context* ctx, std::string param) {
	std::string value = EnvSystem::Get(param);
	return value;
}

std::string
ContextSystem::CMD_SETENV(Context* ctx, std::string param) {
	std::vector<std::string> substr_vecotr;
	UtilsSystem::SplitString(param, ':', substr_vecotr);
	if (substr_vecotr.size() == 2) {
		EnvSystem::Set(substr_vecotr.at(0), substr_vecotr.at(1));
		return substr_vecotr.at(1);
	}
	else {
		return "";
	}
}

std::string
ContextSystem::CMD_EXIT(Context* ctx, std::string param) {
	ctx->data_->dead_ = true;
	return "1";
}

std::string
ContextSystem::CMD_KILL(Context* ctx, std::string param) {
	uint32_t handle = std::stoi(param);
	Context* kill_ctx = HandleSystem::FindContext(handle);
	if (kill_ctx != nullptr) {
		kill_ctx->data_->dead_ = true;
		return "1";
	}
	else {
		return "0";
	}
}

std::string
ContextSystem::CMD_LAUNCH(Context* ctx, std::string param) {
	std::vector<std::string> substr_vecotr;
	UtilsSystem::SplitString(param, ' ', substr_vecotr);
	if (substr_vecotr.size() != 2) {
		return "0";
	}

	Context* new_ctx = ContextSystem::New(substr_vecotr.at(0), substr_vecotr.at(1));
	if (new_ctx != nullptr) {
		return UtilsSystem::FormatString("%ud", new_ctx->get_handle(new_ctx));
	}
	else {
		return "0";
	}
}

std::string
ContextSystem::CMD_TIMEOUT(Context* ctx, std::string param) {
	size_t time = std::stoi(param);
	int session = TimerSystem::Timeout(ctx->get_handle(ctx), PTYPE_RESPONSE, ContextSystem::NewSession(ctx), nullptr, 0, time);
	return UtilsSystem::FormatString("%d", session);
}

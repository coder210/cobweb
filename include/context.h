#ifndef _CONTEX_H
#define _CONTEX_H
#include <string>
#include <cstdint>
#include "mq.h"
#include "module.h"
extern "C" {
#include "lua/lapi.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

#ifdef _COBWEB_WIN32
#define COBWEB_MOD_API extern "C" __declspec(dllexport)
#else
#define COBWEB_MOD_API extern "C"
#endif

#define SYSTEM_HANDLE 0

#define PTYPE_TEXT 0
#define PTYPE_RESPONSE 1
#define PTYPE_MULTICAST 2
#define PTYPE_CLIENT 3
#define PTYPE_SYSTEM 4
#define PTYPE_SOCKET 5
#define PTYPE_ERROR 6
#define PTYPE_DEBUG 7
#define PTYPE_LUA 8

#define PTYPE_TAG_ALLOCSESSION 0x10000
#define PTYPE_TAG_DONTCOPY 0x20000
#define PTYPE_TAG_STRING 0x30000
#define PTYPE_TAG_USERDATA 0x40000

#define SOCKET_TYPE_DATA 101
#define SOCKET_TYPE_CONNECT 102
#define SOCKET_TYPE_CLOSE 103
#define SOCKET_TYPE_ACCEPT 104
#define SOCKET_TYPE_ERROR 105
#define SOCKET_TYPE_WARNING 106

struct SocketMessage {
	int stype_;
	void* data_;
};

struct Context;

struct ContextData;

using context_callback =  bool (*)(Context*, void*, int, int, uint32_t, const void*, size_t);
using api_callback = void (*)(Context*, void*, context_callback);
using api_command = std::string (*)(Context*, std::string, std::string);
using api_info = void (*)(const char* format, ...);
using api_debug = void (*)(const char* format, ...);
using api_error = void (*)(const char* format, ...);
using api_log = void (*)(uint32_t, const char* format, ...);
using api_log_error = void (*)(uint32_t, const char* format, ...);
using api_timestamp = int64_t(*)(void);
using api_current_datetime = std::string(*)(const char*);
using api_timeout = int (*)(uint32_t, int, int, const void*, size_t, size_t);
using api_get_handle = uint32_t (*)(Context*);
using api_push = bool (*)(Context*, uint32_t, int, int, const void*, size_t);
using api_send = bool (*)(uint32_t, uint32_t, int, int, const void*, size_t);
using api_sendname = bool (*)(uint32_t, std::string, int, int, const void*, size_t);
using api_newsession = int (*)(Context*);
using api_version = std::string (*)(void);

struct Context {
	ContextData* data_;
	api_callback callback;
	api_command command;
	api_info info;
	api_debug debug;
	api_error error;
	api_log log;
	api_log_error log_error;
	api_timestamp timestamp;
	api_current_datetime current_datetime;
	api_timeout timeout;
	api_get_handle get_handle;
	api_push push;
	api_send send;
	api_sendname sendname;
	api_newsession newsession;
	api_version version;
};

class ContextSystem {
public:
	static std::string Version();
	static uint32_t get_handle(Context*);
	static Context* New(std::string, std::string);
	static void Release(Context*);
	static void DispatchMessage(Context*, int);
	static bool Push(Context*, uint32_t, int, int, const void*, size_t);
	static bool Send(uint32_t, uint32_t, int, int, const void*, size_t);
	static bool Sendname(uint32_t, std::string, int, int, const void*, size_t);
	static void Callback(Context*, void*, context_callback);
	static std::string Command(Context*, std::string, std::string);
	static int NewSession(Context*);
	static void Log(uint32_t, const char*, ...);
	static void LogError(uint32_t, const char*, ...);
private:
	static std::string CMD_REG(Context*, std::string);
	static std::string CMD_QUERY(Context*, std::string);
	static std::string CMD_NAME(Context*, std::string);
	static std::string CMD_GETENV(Context*, std::string);
	static std::string CMD_SETENV(Context*, std::string);
	static std::string CMD_EXIT(Context*, std::string);
	static std::string CMD_KILL(Context*, std::string);
	static std::string CMD_LAUNCH(Context*, std::string);
	static std::string CMD_TIMEOUT(Context*, std::string);
};

#endif

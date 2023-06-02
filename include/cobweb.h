#ifndef _COBWEB_H
#define _COBWEB_H
#include <string>
extern "C" {
#include "lua/lapi.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

struct Config {
	int thread_;
	std::string module_path_;
	std::string bootstrap_;
	std::string logservice_;
	std::string project_;
};

class CobwebSystem {
public:
	static int Start(const char*);
private:
	static void PrintBlessing();
	static int OptInt(std::string, int);
	static std::string OptString(std::string, std::string);
	static void InitEnv(lua_State*);
	static void MonitorError(std::string, void*);
	static void ThreadMonitor(void*);
	static void ThreadTimer(void*);
	static void ThreadMain(void*);
	static void ThreadWorker(void*);
};

#endif

#ifndef _MODULE_H
#define _MODULE_H
#include <cassert>
#include <list>
#include <vector>
#include <string>
#include <mutex>

typedef void* (*dl_create)(void);
typedef bool (*dl_init)(void* inst, void* ctx, std::string param);
typedef void (*dl_release)(void* inst);
typedef void (*dl_signal)(void* inst, int signal);

class Module {
public:
	std::string name_;
	void* dl_;
	dl_create create_;
	dl_init init_;
	dl_release release_;
	dl_signal signal_;
};

class ModuleSystem {
public:
	static void Init(std::string path);
	static void Release(void);
	static Module* Query(std::string name);
	static void* Open(std::string name);
	static void* InstanceCreate(Module* module);
	static bool InstanceInit(Module* module, void* inst, void* ctx, std::string param);
	static void InstanceRelease(Module* module, void* inst);
	static void InstanceSignal(Module* module, void* inst, int signal);
};

#endif
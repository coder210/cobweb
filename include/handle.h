#ifndef _HANDLE_H
#define _HANDLE_H

#include <cstdint>
#include <string>
#include "cobweb.h"

class HandleSystem
{
public:
	static void Init();
	static void Release();
	static std::string FindName(uint32_t);
	static Context* FindContext(uint32_t);
	static uint32_t FindHandle(std::string);
	static uint32_t Register(Context*);
	static std::string Naming(uint32_t, std::string);
	static void Retire(uint32_t);
	static uint32_t Dequeue();
	static void Enqueue(uint32_t);
};

#endif
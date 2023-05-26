/****************************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_platform.cpp
Description: 跨平台方法
Author: ydlc
Version: 1.0
Date: 2021.4.22
History:
*****************************************************/
#include "platform.h"
#include <cstdarg>
#include <cstdio>
#include <string>
#include <vector>


#ifdef _COBWEB_WIN32
#include <Windows.h>
#pragma comment(lib,"ws2_32.lib")
enum FG_COLOR
{
	FG_NONE = 0x1111,
	FG_BLACK = 0x0,
	FG_RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
	FG_GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
	FG_YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
	FG_BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
	FG_PURPLE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
	FG_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
};

enum BG_COLOR
{
	BG_NONE = 0x1111,
	BG_BLACK = 0x0,
	BG_RED = BACKGROUND_INTENSITY | BACKGROUND_RED,
	BG_GREEN = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
	BG_BLUE = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
	BG_YELLOW = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
	BG_PURPLE = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
	BG_WHITE = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
};

#elif _COBWEB_LINUX
#include<unistd.h>
#include<dlfcn.h>
#include<errno.h>

enum FG_COLOR 
{
	FG_NONE = 0,
	FG_BLACK = 30,
	FG_RED = 31,
	FG_GREEN = 32,
	FG_YELLOW = 33,
	FG_BLUE = 34,
	FG_PURPLE = 35,
	FG_WHITE = 37
};

enum BG_COLOR 
{
	BG_NONE = 0,
	BG_BLACK = 40,
	BG_RED = 41,
	BG_GREEN = 42,
	BG_YELLOW = 43,
	BG_BLUE = 44,
	BG_PURPLE = 45,
	BG_WHITE = 47
};

#else

#endif


void*
PlatformSystem::dllopen(const char* file) {
#ifdef _COBWEB_WIN32
	void* dl = (void*)LoadLibraryA(file);
#elif _COBWEB_LINUX
	void* dl = dlopen(file, RTLD_NOW | RTLD_GLOBAL);
#else
	void* dl = nullptr;
#endif
	return dl;
}

void*
PlatformSystem::dllsym(void* dl, const char* name) {
#ifdef _COBWEB_WIN32
	void* address = (void*)GetProcAddress((HMODULE)dl, name);
#elif _COBWEB_LINUX
	void* address = dlsym(dl, name);
#else
	void* address = nullptr;
#endif
	return address;
}

int
PlatformSystem::dllclose(void* dl) {
#ifdef _COBWEB_WIN32
	bool ret = FreeLibrary((HMODULE)dl);
#elif _COBWEB_LINUX
	bool ret = (dlclose(dl) == 0);
#else
	bool ret = false;
#endif
	return ret;
}

void
PlatformSystem::dllerror(char* msg) {
#ifdef _COBWEB_WIN32
	int errorno = GetLastError();
	sprintf(msg, "error number is %d", errorno);
#elif _COBWEB_LINUX
	sprintf(msg, "%s", dlerror());
#else

#endif
}

void
PlatformSystem::printc(int fg_color, const char* str) {
#ifdef _COBWEB_WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), fg_color);
	printf("%s\n", str);
#elif _COBWEB_LINUX
	printf("\033[%d;%dm%s\033[0m\n", BG_NONE, fg_color, str);
#else
	printf("%s\n", str);
#endif
}

void
PlatformSystem::info(const char* format, ...) {
	std::string str;
	va_list ap;
	va_start(ap, format);
	int length = _vscprintf(format, ap) + 1;
	std::vector<char> char_vector(length);
	_vsnprintf(char_vector.data(), length, format, ap);
	str.assign(char_vector.data());
	va_end(ap);
	PlatformSystem::printc(FG_GREEN, str.c_str());
}

void
PlatformSystem::debug(const char* format, ...) {
	std::string str;
	va_list ap;
	va_start(ap, format);
	int length = _vscprintf(format, ap) + 1;
	std::vector<char> char_vector(length);
	_vsnprintf(char_vector.data(), length, format, ap);
	str.assign(char_vector.data());
	va_end(ap);
	PlatformSystem::printc(FG_BLUE, str.c_str());
}

void
PlatformSystem::error(const char* format, ...) {
	std::string str;
	va_list ap;
	va_start(ap, format);
	int length = _vscprintf(format, ap) + 1;
	std::vector<char> char_vector(length);
	_vsnprintf(char_vector.data(), length, format, ap);
	str.assign(char_vector.data());
	va_end(ap);
	PlatformSystem::printc(FG_RED, str.c_str());
}

void
PlatformSystem::msleep(int msec) {
#ifdef _COBWEB_WIN32
	Sleep(msec);
#elif _COBWEB_LINUX
	usleep((msec << 10) - (msec << 4) - (msec << 3));
#endif
}

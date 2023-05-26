#ifndef _PLATFORM_H
#define _PLATFORM_H


class PlatformSystem {
public:
	static void* dllopen(const char* filename);
	static void* dllsym(void* dl, const char* filename);
	static int dllclose(void* dl);
	static void dllerror(char* msg);
	static void printc(int fg_color, const char* str);
	static void info(const char* format, ...);
	static void debug(const char* format, ...);
	static void error(const char* format, ...);
	static void msleep(int msec);
};


#endif
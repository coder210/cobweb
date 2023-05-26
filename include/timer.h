#ifndef _TIMER_H
#define _TIMER_H
#include <vector>
#include <list>
#include <mutex>

class TimerSystem {
public:
	static void Init();
	static void Release();
	static int Timeout(uint32_t, int, int, const void*, size_t, size_t);
	static void Tick();
	static void Sleep(void);
};

#endif
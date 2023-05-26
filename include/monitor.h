#ifndef _MONITOR_H
#define _MONITOR_H

#include <cstdint>
#include <string>

class Monitor {
public:
	int version_;
	int check_version_;
	uint32_t source_;
	uint32_t dest_;
};

class MonitorSystem {
public:
	static void Init(int);
	static void Release(void);
	static void Sleep(void);
	static void Inspect(void(*error_msg)(std::string, void*), void*);
	static bool get_quit(void);
	static void Quit(void);
	static void Wait(void);
	static void Wakeup(void);
	static void Trigger(int, uint32_t, uint32_t);
private:
	static bool is_continue();
};

#endif
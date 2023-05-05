/************************************************
Copyright: 2021-2022, lanchong.xyz/Ltd.
File name: platform_COBWEB_LINUX.c
Description: 导出的接口
Author: ydlc
Version: 1.0
Date: 2021.12.12
History:
************************************************/
#include "platform.h"
#include <stdio.h>
#include <stdarg.h>


#define CC_TIMER_INTERVAL 10

#ifdef _COBWEB_WIN32

int
ccp_daemon_init(const char* pidfile) {
	return 0;
}

int
ccp_daemon_exit(const char* pidfile) {
	return 0;
}

void
ccp_exception_debug(int signo) {

}

int
ccp_timefd_create(void) {
	return 0;
}

int
ccp_timer_timeout(int timerfd) {
	ccp_msleep(CC_TIMER_INTERVAL);
	return 1;
}

void
ccp_timerfd_close(int timerfd) {
}


#endif

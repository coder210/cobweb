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


#ifdef _COBWEB_LINUX
#define __USE_POSIX199309
#include <unistd.h>
#include <sys/types.h>
#include <sys/file.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/timerfd.h>
#endif


#ifdef _COBWEB_LINUX

static int
_check_pid(const char* pidfile) {
	int pid = 0;
	FILE* f = fopen(pidfile, "r");
	if (f == NULL)
		return 0;
	int n = fscanf(f, "%d", &pid);
	fclose(f);

	if (n != 1 || pid == 0 || pid == getpid()) {
		return 0;
	}

	if (kill(pid, 0) && errno == ESRCH)
		return 0;

	return pid;
}

static int
_write_pid(const char* pidfile) {
	FILE* f;
	int pid = 0;
	int fd = open(pidfile, O_RDWR | O_CREAT, 0644);
	if (fd == -1) {
		ccp_error("Can't create %s.", pidfile);
		return 0;
	}
	f = fdopen(fd, "r+");
	if (f == NULL) {
		ccp_error("Can't open %s.", pidfile);
		return 0;
	}

	if (flock(fd, LOCK_EX | LOCK_NB) == -1) {
		int n = fscanf(f, "%d", &pid);
		fclose(f);
		if (n != 1) {
			ccp_red_print("Can't lock and read pidfile.");
		}
		else {
			ccp_error("Can't lock pidfile, lock is held by pid %d.", pid);
		}
		return 0;
	}

	pid = getpid();
	if (!fprintf(f, "%d\n", pid)) {
		ccp_red_print("Can't write pid.");
		close(fd);
		return 0;
	}
	fflush(f);

	return pid;
}

int
ccp_daemon_init(const char* pidfile) {
	int pid = _check_pid(pidfile);

	if (pid) {
		ccx_error("%s is already running, pid = %d.", pidfile, pid);
		return 1;
	}

#ifdef __APPLE__
	ccx_error("'daemon' is deprecated: first deprecated in OS X 10.5 , use launchd instead.");
#else
	if (daemon(1, 0)) {
		ccx_error("Can't daemonize.");
		return 1;
	}
#endif

	pid = _write_pid(pidfile);
	if (pid == 0) {
		return 1;
	}

	return 0;
}

int
ccp_daemon_exit(const char* pidfile) {
	return unlink(pidfile);
}

void
ccp_exception_debug(int signo) {
	char filename[CC_MAX_PATH];
	sprintf(filename, "%s/%s.log", "dump", ccp_datetime("%Y-%m-%d").buffer);

	// write title
	FILE* fp = fopen(filename, "a+");
	if (fp != NULL) {
		fprintf(fp, "\n=========>>>%s catch signal %d <<<=========\n", ccp_datetime("%Y-%m-%d %I:%M:%S").buffer, signo);
		fclose(fp);
	}

	// write expection
	fp = fopen(filename, "a+");
	if (fp != NULL) {
		//std_backtrace(fp);
		fclose(fp);
	}

	signal(signo, SIG_DFL); /* 恢复信号默认处理 */
	raise(signo);           /* 重新发送信号 */
}


int
ccp_timefd_create(void) {
	int timerfd = timerfd_create(CLOCK_MONOTONIC, 0);

	/*设置定时周期,使用纳秒*/
	long tmp_interval = CC_TIMER_INTERVAL * 1000000;
	long tmp_value = CC_TIMER_INTERVAL * 1000000;
	int sec = 0;
	if (1000000000 >= tmp_interval) {
		sec = tmp_interval / 1000000000;
		tmp_interval = tmp_interval % 1000000000;
		tmp_value = tmp_value % 1000000000;
	}

	struct itimerspec period = {
		{sec,tmp_interval}, {sec,tmp_value}
	};

	if (timerfd_settime(timerfd, 0, &period, NULL) < 0) {
		ccp_red_print("timerfd_settime error!");
	}

	return timerfd;
}

int
ccp_timer_timeout(int timerfd) {
	uint64_t count = 0;
	int timeout = 0;
	if (read(timerfd, &count, sizeof(uint64_t)) == -1) {
		return timeout;
	}
	else {
		timeout = (int)count;
		return timeout;
	}
}

void
ccp_timerfd_close(int timerfd) {
	close(timerfd);
}

#endif



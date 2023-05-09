/************************************************
Copyright: 2021-2022, lanchong.xyz/Ltd.
File name: cobweb_start.c
Description: 开始启动服务器
Author: ydlc
Version: 1.0
Date: 2021.12.14
History:
*************************************************/

#include "cobweb.h"
#include <thread>
#include <chrono>
#include <vector>
extern "C" {
#include "lua/lapi.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}


static void
thread_monitor(void* p) {
	while (!cobweb_monitor_quit()) {
		cobweb_monitor_check();
		cobweb_monitor_sleep();
	}
}

static void
thread_timer(void* p) {
	while (!cobweb_monitor_quit()) {
		cobweb_timer_tick(nullptr);
		cobweb_timer_sleep();
	}
}


static void
thread_worker(void* p) {
	struct monitor_t* m = (struct monitor_t*)p;
	while (!cobweb_monitor_quit()) {
		struct context_t* ctx = cobweb_cq_dq();
		if (ctx != nullptr) {
			if (cobweb_dispatch_message(m, ctx)) {
				cobweb_cq_eq(ctx);
			}
		}
		else {
			cobweb_monitor_wait();
		}
	}
}

static void
_start(int thread) {
	std::vector<std::thread> thread_vector;
	thread_vector.push_back(std::thread(thread_monitor, nullptr));
	thread_vector.push_back(std::thread(thread_timer, nullptr));
	for (int i = 0; i < thread; i++) {
		struct monitor_t* m = cobweb_monitor_new();
		thread_vector.push_back(std::thread(thread_worker, m));
	}
	for (size_t i = 0; i < thread_vector.size(); i++) {
		thread_vector.at(i).join();
	}
}

static void
_print_blessing() {
	cobweb_context_log(NULL, "                 _ooOoo_  ");
	cobweb_context_log(NULL, "                o8888888o  ");
	cobweb_context_log(NULL, "                88\" . \"88    ");
	cobweb_context_log(NULL, "                (| -_- |)    ");
	cobweb_context_log(NULL, "             ____/`---'\\____   ");
	cobweb_context_log(NULL, "           .'  \\|     |//  `.    ");
	cobweb_context_log(NULL, "           /  \\|||  :  |||//  \\    ");
	cobweb_context_log(NULL, "          /  _||||| -:- |||||-  \\    ");
	cobweb_context_log(NULL, "          |   | \\\\  -  /// |   |    ");
	cobweb_context_log(NULL, "         | \\_|  ''\\---/''  |   |   ");
	cobweb_context_log(NULL, "          \\  .-\\__  `-`  ___/-. /    ");
	cobweb_context_log(NULL, "         ___`. .'  /--.--\\  `. . __    ");
	cobweb_context_log(NULL, "      .\"\" '<  `.___\\_<|>_/___.'  >'\"\".    ");
	cobweb_context_log(NULL, "     | | :  `- \\`.;`\\ _ /`;.`/ - ` : | |    ");
	cobweb_context_log(NULL, "   \\  \\ `-.   \\_ __\\ /__ _/   .-` /  /    ");
	cobweb_context_log(NULL, "  =====`-.____`-.___\\_____/___.-`____.-'======    ");
	cobweb_context_log(NULL, " `^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^      ");
	cobweb_context_log(NULL, " `          佛祖保佑       永无Bug     ");
	cobweb_context_log(NULL, " `          快加工资       不改需求      ");
}

void
cobweb_start(struct config_t* config) {
	cobweb_cq_init();
	cobweb_module_init(config->module_path);
	cobweb_monitor_init();
	cobweb_handle_init();
	cobweb_timer_init();

	/* lunch logger */
	const char* project = cobweb_getenv("project");
	if (cobweb_context_new(config->logservice, project) == NULL) {
		platform_error("Can't launch %s service", config->logservice);
		return;
	}

	/* print blessing */
	_print_blessing();

	/* print root dir */
	cobweb_context_log(NULL, "work path is %s", cobweb_getenv("absolut_path"));

	/* lunch bootstrap */
	const char* bootstrap = cobweb_getenv("bootstrap");
	if (cobweb_context_new("luax", bootstrap) == NULL) {
		cobweb_context_log(NULL, "Can't launch %s service", bootstrap);
		return;
	}

	_start(config->thread);

	cobweb_timer_release();
	cobweb_cq_release();
	cobweb_module_release();
	cobweb_monitor_release();
	cobweb_handle_release();
}


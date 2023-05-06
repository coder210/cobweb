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
extern "C" {
#include "lua/lapi.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}


#define CHECK_ABORT if (1==0) break;


struct worker_parm {
	struct monitor_t* m;
	int id;
};


static void*
thread_monitor(void* p) {
	int i;
	while (!cobweb_monitor_quit()) {
		int n = cobweb_monitor_count();
		for (i = 0; i < n; i++) {
			struct monitor_t* sm = cobweb_query_monitor(i);
			if (!cobweb_monitor_check(sm)) {
				cobweb_monitor_error(sm);
			}
		}
		for (i = 0; i < n; i++) {
			CHECK_ABORT
			platform_msleep(1000);
		}

		cobweb_monitor_wakeup(0);
	}

	return NULL;
}

bool
_tick_cb(uint32_t handle, int session, 
void* data, size_t sz, void* arg) {
	struct context_t* ctx = cobweb_handle_grab(handle);
	if (ctx != NULL) {
		cobweb_context_send(ctx, 0, handle, PTYPE_RESPONSE, session, data, sz);
		return true;
	}
	else {
		return false;
	}
}

static void*
thread_timer(void* p) {
	while (!cobweb_monitor_quit()) {
		CHECK_ABORT
		cobweb_timer_tick(_tick_cb, NULL);
	}
	return NULL;
}

static void*
thread_socket(void* p) {
	if (!cobweb_monitor_quit()) {
		cobweb_sserver_dispatch();
	}
	return NULL;
}

static void*
thread_main(void* p) {
	/* start script */
	char start_file[512] = { 0 };
	sprintf(start_file, "%s/%s.lua", cobweb_getenv("project"), cobweb_getenv("start"));
	struct lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	int err = luaL_dofile(L, start_file);
	if (err) {
		platform_red_print(lua_tostring(L, -1));
	}
	else {
		lua_close(L);
	}
	return NULL;
}

static void*
thread_worker(void* p) {
	struct worker_parm* wp = (struct worker_parm*)p;
	struct monitor_t* sm = cobweb_query_monitor(wp->id);
	while (!cobweb_monitor_quit()) {
		struct mq_t* mq = cobweb_globalmq_dq();
		if (mq != NULL) {
			uint32_t handle = cobweb_mq_handle(mq);
			struct context_t* ctx = cobweb_handle_grab(handle);
			if (ctx != NULL && cobweb_context_action(ctx, sm)) {
				cobweb_globalmq_eq(mq);
			}
		}
		else {
			cobweb_monitor_wait();
		}
	}
	return NULL;
}

static void
_start(int thread) {
	int total_thread = thread + 4;
	struct thread_t* pid = (struct thread_t*)cobweb_malloca(sizeof(struct thread_t) * total_thread);
	assert(pid != NULL);
	struct worker_parm* pwp = (struct worker_parm*)cobweb_malloca(sizeof(struct worker_parm) * thread);
	assert(pwp != NULL);
	pid[0] = platform_thread_create(thread_monitor, NULL);
	pid[1] = platform_thread_create(thread_timer, NULL);
	pid[2] = platform_thread_create(thread_socket, NULL);
	pid[3] = platform_thread_create(thread_main, NULL);
	for (int i = 0; i < thread; i++) {
		pwp[i].m = cobweb_query_monitor(i);
		pwp[i].id = i;
		pid[4 + i] = platform_thread_create(thread_worker, &pwp[i]);
	}
	for (int i = 0; i < total_thread; i++) {
		platform_thread_wait(&pid[i]);
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
	cobweb_globalmq_init();
	cobweb_module_init(config->module_path);
	cobweb_monitor_init(config->thread);
	cobweb_handle_init(config->harbor);
	cobweb_timer_init();
	cobweb_sserver_init((char*)config->address, config->afterend, config->proto_parse);

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

	/* lunch gate */
	if (cobweb_context_new("gate", NULL) == NULL) {
		cobweb_context_log(NULL, "Can't launch gate service");
		return;
	}

	/* lunch bootstrap */
	const char* bootstrap = cobweb_getenv("bootstrap");
	if (cobweb_context_new("luax", bootstrap) == NULL) {
		cobweb_context_log(NULL, "Can't launch %s service", bootstrap);
		return;
	}

	_start(config->thread);

	cobweb_sserver_release();
	cobweb_timer_release();
	cobweb_globalmq_release();
	cobweb_module_release();
	cobweb_monitor_release();
	cobweb_handle_release();
}


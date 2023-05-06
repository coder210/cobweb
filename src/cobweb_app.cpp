/**
						 _ooOoo_
						o8888888o
						88\" . \"88
						(| -_- |)
					 ____/`---'\\____
				   .'  \\|     |//  `.
				   /  \\|||  :  |||//  \\
				  /  _||||| -:- |||||-  \\
				  |   | \\\\  -  /// |   |
				 | \\_|  ''\\---/''  |   |
				  \\  .-\\__  `-`  ___/-. /
				 ___`. .'  /--.--\\  `. . __
			  .\"\" '<  `.___\\_<|>_/___.'  >'\"\".
			 | | :  `- \\`.;`\\ _ /`;.`/ - ` : | |
		   \\  \\ `-.   \\_ __\\ /__ _/   .-` /  /
		  =====`-.____`-.___\\_____/___.-`____.-'======
		 `^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		 `          佛祖保佑       永无Bug
		 `          快加工资       不改需求
		  佛曰:
				  写字楼里写字间，写字间里程序员；
				  程序人员写程序，又拿程序换酒钱。
				  酒醒只在网上坐，酒醉还来网下眠；
				  酒醉酒醒日复日，网上网下年复年。
				  但愿老死电脑间，不愿鞠躬老板前；
				  奔驰宝马贵者趣，公交自行程序员。
				  别人笑我忒疯癫，我笑自己命太贱；
*/

#include "cobweb.h"
extern "C" {
#include "lua/lapi.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}


static int
_optint(const char* key, int opt) {
	const char* str = cobweb_getenv(key);
	if (str == NULL) {
		char tmp[20];
		sprintf(tmp, "%d", opt);
		cobweb_setenv(key, tmp);
		return opt;
	}
	return (int)strtol(str, NULL, 10);
}

static const char*
_optstring(const char* key, const char* opt) {
	const char* str = cobweb_getenv(key);
	if (str == NULL) {
		if (opt) {
			cobweb_setenv(key, opt);
			opt = cobweb_getenv(key);
		}
		return opt;
	}
	return str;
}

static void
_init_env(lua_State* L) {
	lua_pushnil(L);  /* first key */
	while (lua_next(L, -2) != 0) {
		int keyt = lua_type(L, -2);
		if (keyt != LUA_TSTRING) {
			platform_log("Invalid config table");
			return;
		}
		const char* key = lua_tostring(L, -2);
		if (lua_type(L, -1) == LUA_TBOOLEAN) {
			int b = lua_toboolean(L, -1);
			cobweb_setenv(key, b ? "true" : "false");
		}
		else {
			const char* value = lua_tostring(L, -1);
			if (value == NULL) {
				platform_error("Invalid config table key = %s", key);
				return;
			}
			cobweb_setenv(key, value);
		}
		lua_pop(L, 1);
	}
	lua_pop(L, 1);
}

int
cobweb_run(const char* config_file) {
	cobweb_env_init();
	struct config_t config;

	struct lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	int err = luaL_dofile(L, config_file);
	if (err) {
		platform_error("%s", lua_tostring(L, -1));
		lua_close(L);
		return 1;
	}

	_init_env(L);

	config.thread = _optint("thread", 8);
	config.module_path = _optstring("cpath", "./cservice/?.so");
	config.harbor = _optint("harbor", 1);
	config.bootstrap = _optstring("bootstrap", "luax bootstrap");
	config.daemon = _optstring("daemon", NULL);
	config.logservice = _optstring("logservice", "logger");
	config.address = _optstring("address", "");
	config.master = _optstring("master", "");
	config.afterend = _optint("afterend", 1);
	config.proto_parse = _optint("parse", 1);

	lua_close(L);

	char tmp[COBWEB_MAX_PATH] = { 0 };
	cobweb_setenv("absolut_path", platform_root_dir(tmp, COBWEB_MAX_PATH));

	/* init daemon */
	if (config.daemon) {
		if (platform_daemon_init(config.daemon)) {
			return 1;
		}
	}

	/* init network */
	platform_network_init();

	/* start */
	cobweb_start(&config);

	/* release network */
	platform_network_release();

	/* exit daemon */
	if (config.daemon) {
		platform_daemon_exit(config.daemon);
	}

	return 0;
}


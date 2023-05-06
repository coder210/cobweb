#include "def.h"
#include "cobweb.h"
extern "C" {
#include "lua/lapi.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

#define MEMORY_WARNING_REPORT (1024 * 1024 * 32)

struct luax {
	lua_State* L; //lua虚拟机
	struct context_t* ctx; //服务ctx
	size_t mem; //当前使用的内存
	size_t mem_report; //内存警告临界值
	size_t mem_limit; //内存使用上线
};

// LUA_CACHELIB may defined in patched lua for shared proto
#ifdef LUA_CACHELIB

#define codecache luaopen_cache

#else

static int
cleardummy(lua_State* L) {
	return 0;
}

static int
codecache(lua_State* L) {
	luaL_Reg l[] = {
		{ "clear", cleardummy },
		{ "mode", cleardummy },
		{ NULL, NULL },
	};
	luaL_newlib(L, l);
	lua_getglobal(L, "loadfile");
	lua_setfield(L, -2, "loadfile");
	return 1;
}

#endif

static int
traceback(lua_State* L) {
	const char* msg = lua_tostring(L, 1);
	if (msg) {
		luaL_traceback(L, L, msg, 1);
	}
	else {
		lua_pushliteral(L, "(no error message)");
	}
	return 1;
}

//static void
//report_launcher_error(struct context_t* ctx) {
//	// sizeof "ERROR" == 5
//	lnE_ctx_sendname(ctx, 0, ".launcher", PTYPE_TEXT, 0, "ERROR", 5);
//}

static const char*
optstring(struct context_t* ctx, const char* key, const char* str) {
	const char* ret = ctx->command(ctx, "GETSTRING", key);
	if (ret == NULL) {
		return str;
	}
	return ret;
}

static int
init_cb(struct luax* l, struct context_t* ctx,
	const char* args, size_t sz) {
	lua_State* L = l->L;
	l->ctx = ctx;
	lua_gc(L, LUA_GCSTOP, 0);
	lua_pushboolean(L, 1);  /* signal for libraries to ignore env. vars. */
	lua_setfield(L, LUA_REGISTRYINDEX, "LUA_NOENV");
	luaL_openlibs(L);
	lua_pushlightuserdata(L, ctx);
	lua_setfield(L, LUA_REGISTRYINDEX, "__this");
	luaL_requiref(L, "cobweb.codecache", codecache, 0);
	//luaL_requiref(L, "engine.core", luaopen_cobweb_core, 0);
	//luaL_requiref(L, "engine.socket", luaopen_cobweb_socket, 0);
	//luaL_requiref(L, "udp", luaopen_udp, 0);
	//luaL_requiref(L, "tcp", luaopen_tcp, 0);
	//luaL_requiref(L, "netpack", luaopen_netpack, 0);
	//luaL_requiref(L, "crypt", luaopen_crypt, 0);
	//luaL_requiref(L, "mysql", luaopen_mysql, 0);

	lua_pop(L, 1);
	
	const char* lua_path = optstring(ctx, "lua_path", NULL);
	const char* lua_cpath = optstring(ctx, "lua_cpath", NULL);
	const char* project = optstring(ctx, "project", NULL);

	lua_pushcfunction(L, traceback);
	assert(lua_gettop(L) == 1);

	const char* loader = "./lualib/loader.lua";
	int r = luaL_loadfile(L, loader);
	if (r != LUA_OK) {
		ctx->log(ctx, "Can't load %s : %s", args, lua_tostring(L, -1));
		return 1;
	}

	lua_pushlstring(L, args, sz);
	lua_pushstring(L, lua_path);
	lua_pushstring(L, lua_cpath);
	lua_pushstring(L, project);
	r = lua_pcall(L, 4, 0, 1);
	if (r != LUA_OK) {
		ctx->log(ctx, "lua loader error : %s", lua_tostring(L, -1));
		return 1;
	}
	lua_settop(L, 0);
	if (lua_getfield(L, LUA_REGISTRYINDEX, "memlimit") == LUA_TNUMBER) {
		size_t limit = lua_tointeger(L, -1);
		l->mem_limit = limit;
		ctx->log(ctx, "Set memory limit to %.2f M", (float)limit / (1024 * 1024));
		lua_pushnil(L);
		lua_setfield(L, LUA_REGISTRYINDEX, "memlimit");
	}
	lua_pop(L, 1);

	lua_gc(L, LUA_GCRESTART, 0);

	return 0;
}

static int
launch_cb(struct context_t* ctx, void* ud, int type, 
	int session, uint32_t source, const void* msg, size_t sz) {
	assert(session == 0);
	struct luax* l = (struct luax*)ud;
	ctx->callback(ctx, NULL, NULL);
	int err = init_cb(l, ctx, (const char*)msg, sz);
	if (err) {
		ctx->command(ctx, "EXIT", NULL);
	}

	return 0;
}

static void*
lalloc(void* ud, void* ptr, size_t osize, size_t nsize) {
	struct luax* l = (struct luax*)ud;
	size_t mem = l->mem;
	l->mem += nsize;
	if (ptr) {
		l->mem -= osize;
	}
	if (l->mem_limit != 0 && l->mem > l->mem_limit) {
		if (ptr == NULL || nsize > osize) {
			l->mem = mem;
			return NULL;
		}
	}
	if (l->mem > l->mem_report) {
		l->mem_report *= 2;
		l->ctx->log(l->ctx, "Memory warning %.2f M", (float)l->mem / (1024 * 1024));
	}
	if (nsize == 0) {
		free(ptr);
		return NULL;
	}
	else {
		return realloc(ptr, nsize);
	}
}

COBWEB_CMOD_API struct luax*
luax_create(void) {
	struct luax* l = (struct luax*)malloc(sizeof(struct luax));
	if (l != NULL) {
		memset(l, 0, sizeof(struct luax));
		l->mem_report = MEMORY_WARNING_REPORT;
		l->mem_limit = 0;
		l->L = lua_newstate(lalloc, l);
	}
	return l;
}

COBWEB_CMOD_API int
luax_init(struct luax* l, struct context_t* ctx, const char* args) {
	int sz = strlen(args);
	char* tmp = (char*)malloc(sz + 1);
	if (tmp != NULL) {
		memcpy(tmp, args, sz);
		tmp[sz] = 0;
		ctx->callback(ctx, l, launch_cb);
		// 调用ccx_context_command时传入null这时并不是注册服务，而是返回服务的句柄
		const char* self = ctx->command(ctx, "REG", NULL);
		uint32_t handle_id = strtoul(self + 1, NULL, 16);
		ctx->send(ctx, 0, handle_id, PTYPE_TAG_DONTCOPY, 0, tmp, sz);
	}
	return 0;
}

COBWEB_CMOD_API void
luax_release(struct luax* l) {
	lua_close(l->L);
	free(l);
}

COBWEB_CMOD_API void
luax_signal(struct luax* l, int signal) {
	l->ctx->log(l->ctx, "recv a signal %d", signal);
	if (signal == 0) {
		//#ifdef lua_checksig
		//	// If our lua support signal (modified lua version by engine), trigger it.
		//	engine_sig_L = l->L;
		//#endif
	}
	else if (signal == 1) {
		l->ctx->log(l->ctx, "Current Memory %.3fK", (float)l->mem / 1024);
	}
}

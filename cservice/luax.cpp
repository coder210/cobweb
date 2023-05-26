#include "cobweb.h"
extern "C" {
#include "lua/lapi.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

#define MEMORY_WARNING_REPORT (1024 * 1024 * 32)

struct luax {
	lua_State* L;
	Context* ctx;
	size_t mem;
	size_t mem_report;
	size_t mem_limit;
};

static void*
_lalloc(void* ud, void* ptr, size_t osize, size_t nsize) {
	luax* l = (luax*)ud;
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
		l->ctx->log_error(l->ctx->get_handle(l->ctx), "Memory warning %.2f M", (float)l->mem / (1024 * 1024));
	}
	if (nsize == 0) {
		free(ptr);
		return NULL;
	}
	else {
		return realloc(ptr, nsize);
	}
}

COBWEB_MOD_API luax*
luax_create(void) {
	luax* l = new luax();
	if (l != nullptr) {
		memset(l, 0, sizeof(luax));
		l->mem_report = MEMORY_WARNING_REPORT;
		l->mem_limit = 0;
	}
	return l;
}

static int
_traceback(lua_State* L) {
	const char* msg = lua_tostring(L, 1);
	if (msg) {
		luaL_traceback(L, L, msg, 1);
	}
	else {
		lua_pushliteral(L, "(no error message)");
	}
	return 1;
}

static bool
_init_cb(luax* l, Context* ctx, const char* param, size_t sz) {
	lua_State* L = l->L;
	lua_gc(L, LUA_GCSTOP, 0);
	lua_pushboolean(L, 1);
	lua_setfield(L, LUA_REGISTRYINDEX, "LUA_NOENV");
	luaL_openlibs(L);
	lua_pushlightuserdata(L, ctx);
	lua_setfield(L, LUA_REGISTRYINDEX, "__this");

	std::string lua_path = ctx->command(ctx, "GETENV", "lua_path");
	std::string lua_cpath = ctx->command(ctx, "GETENV", "lua_cpath");
	std::string project = ctx->command(ctx, "GETENV", "project");

	lua_pushcfunction(L, _traceback);
	assert(lua_gettop(L) == 1);

	const char* loader = "./lualib/loader.lua";
	int r = luaL_loadfile(L, loader);
	if (r != LUA_OK) {
		ctx->log_error(ctx->get_handle(ctx), "Can't load %s : %s", param, lua_tostring(L, -1));
		return false;
	}
	lua_pushlstring(L, param, sz);
	lua_pushstring(L, lua_path.c_str());
	lua_pushstring(L, lua_cpath.c_str());
	lua_pushstring(L, project.c_str());
	r = lua_pcall(L, 4, 0, 1);
	if (r != LUA_OK) {
		ctx->log_error(ctx->get_handle(ctx), "lua loader error : %s", lua_tostring(L, -1));
		return 1;
	}
	lua_settop(L, 0);
	if (lua_getfield(L, LUA_REGISTRYINDEX, "memlimit") == LUA_TNUMBER) {
		size_t limit = (size_t)lua_tointeger(L, -1);
		l->mem_limit = limit;
		ctx->log_error(ctx->get_handle(ctx), "Set memory limit to %.2f M", (float)limit / (1024 * 1024));
		lua_pushnil(L);
		lua_setfield(L, LUA_REGISTRYINDEX, "memlimit");
	}
	lua_pop(L, 1);

	lua_gc(L, LUA_GCRESTART, 0);

	return true;
}

static bool
_launch_cb(Context* ctx, void* ud, int type, int session, uint32_t source, const void* data, size_t sz) {
	assert(session == 0);
	luax* l = (luax*)ud;
	ctx->callback(ctx, nullptr, nullptr);
	if (!_init_cb(l, ctx, (const char*)data, sz)) {
		ctx->command(ctx, "EXIT", "");
	}
	return 0;
}

COBWEB_MOD_API bool
luax_init(luax* l, Context* ctx, std::string param) {
	l->ctx = ctx;
	l->L = lua_newstate(_lalloc, l);
	l->ctx->callback(ctx, l, _launch_cb);
	l->ctx->push(ctx, 0, PTYPE_RESPONSE | PTYPE_TAG_STRING, 0, param.c_str(), param.size());
	return true;
}

COBWEB_MOD_API void
luax_release(luax* l) {
	lua_close(l->L);
	delete l;
}

COBWEB_MOD_API void
luax_signal(luax* l, int signal) {

}

/************************************************
Copyright: 2021-2022, lanchong.xyz/Ltd.
File name: cobweb_env.c
Description: 存储变量的值
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

// var
struct env_t {
	struct mutex_t* mutex;
	lua_State* L;
};


static struct env_t* E = NULL;


void
cobweb_env_init(void) {
	E = (struct env_t*)cobweb_malloc(sizeof(struct env_t));
	if (E != NULL) {
		E->mutex = ccp_mutex_create();
		E->L = luaL_newstate();
	}
}

const char*
cobweb_getenv(const char* key) {
	ccp_mutex_lock(E->mutex);

	lua_State* L = E->L;
	lua_getglobal(L, key);
	const char* result = lua_tostring(L, -1);
	lua_pop(L, 1);

	ccp_mutex_unlock(E->mutex);

	return result;
}

void
cobweb_setenv(const char* key, const char* value) {
	ccp_mutex_lock(E->mutex);

	lua_State* L = E->L;
	lua_getglobal(L, key);
	lua_pop(L, 1);
	lua_pushstring(L, value);
	lua_setglobal(L, key);

	ccp_mutex_unlock(E->mutex);
}

void
cobweb_env_release(void) {
	if (E != NULL) {
		lua_close(E->L);
		ccp_mutex_release(E->mutex);
		cobweb_free (E);
		E = NULL;
	}
}


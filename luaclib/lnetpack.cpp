#include <stdint.h>
#include <memory.h>
#include <malloc.h>
#include "def.h"
extern "C" {
#include "lua/lapi.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}


static inline void
write_size(uint8_t* buffer, int len) {
	buffer[0] = (len >> 8) & 0xff;
	buffer[1] = len & 0xff;
}

static const char*
tolstring(lua_State* L, size_t* sz, int index) {
	const char* ptr;
	if (lua_isuserdata(L, index)) {
		ptr = (const char*)lua_touserdata(L, index);
		*sz = (size_t)(int)luaL_checkinteger(L, index + 1);
	}
	else {
		ptr = luaL_checklstring(L, index, sz);
	}
	return ptr;
}

static int
lnetpack_pack(struct lua_State* L) {
	size_t len;
	const char* ptr = tolstring(L, &len, 1);
	if (len >= 0x10000) {
		return luaL_error(L, "Invalid size (too long) of data : %d", (int)len);
	}

	uint8_t* buffer = (uint8_t*)malloc(len + 2);
	write_size(buffer, len);
	memcpy(buffer + 2, ptr, len);

	lua_pushlightuserdata(L, buffer);
	lua_pushinteger(L, len + 2);

	return 2;
}

static int
lnetpack_unpack(struct lua_State* L) {
	size_t len = 0;
	const char* ptr = luaL_checklstring(L, 1, &len);
	short size = ptr[1] & 0x000000FF;
	size |= ((ptr[0] << 8) & 0x0000FF00);
	lua_pushinteger(L, size);
	return 1;
}

static int
lnetpack_free(struct lua_State* L) {
	char* ptr = (char*)lua_touserdata(L, 1);
	if (ptr != NULL) {
		free(ptr);
	}
	return 0;
}

static int
lnetpack_tostring(lua_State* L) {
	void* ptr = lua_touserdata(L, 1);
	int size = (int)luaL_checkinteger(L, 2);
	if (ptr == NULL) {
		lua_pushliteral(L, "");
	}
	else {
		lua_pushlstring(L, (const char*)ptr, size);
		free(ptr);
	}
	return 1;
}

COBWEB_CMOD_API int
luaopen_netpack(lua_State* L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{"pack", lnetpack_pack},
		{"unpack", lnetpack_unpack},
		{"tostring", lnetpack_tostring},
		{"free", lnetpack_free},
		{NULL, NULL}
	};
	luaL_newlib(L, l);
	return 1;
}



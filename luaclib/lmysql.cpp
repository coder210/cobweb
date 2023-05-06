#include "def.h"
#include "cobweb.h"
extern "C" {
#include "lua/lapi.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}


#ifdef _COBWEB_WIN32
#include <WinSock2.h>
#pragma comment(lib, "libmysql.lib")
#else
#endif

#include "mysql/mysql.h"


#define MAX_FIELD_LENGTH 256


static int
lmysql_create(struct lua_State* L) {
	const char* host = luaL_checkstring(L, 1);
	const char* user = luaL_checkstring(L, 2);
	const char* passwd = luaL_checkstring(L, 3);
	const char* db = luaL_checkstring(L, 4);
	unsigned int port = (unsigned int)luaL_checkinteger(L, 5);
	MYSQL* context = mysql_init(NULL);
	if (!mysql_real_connect(context, host, user, passwd, db, port, NULL, 0)) {
		lua_pushboolean(L, false);
		lua_pushlightuserdata(L, context);
		lua_pushstring(L, mysql_error(context));
	}
	else {
		lua_pushboolean(L, true);
		lua_pushlightuserdata(L, context);
		lua_pushstring(L, "mysql connect success");
	}
	return 3;
}

static int
lmysql_noquery(struct lua_State* L) {
	MYSQL* context = (MYSQL*)lua_touserdata(L, 1);
	const char* sql = luaL_checkstring(L, 2);
	if (mysql_query(context, sql)) {
		lua_pushinteger(L, 0);
		lua_pushstring(L, mysql_error(context));
	}
	else {
		lua_pushinteger(L, mysql_affected_rows(context));
		lua_pushstring(L, mysql_error(context));
	}

	return 2;
}

static int
lmysql_query(struct lua_State* L) {
	MYSQL* context = (MYSQL*)lua_touserdata(L, 1);
	const char* sql = luaL_checkstring(L, 2);
	if (mysql_query(context, sql)) {
		lua_pushnil(L);
	}
	else {
		MYSQL_RES* res = mysql_store_result(context);
		int num_fields = mysql_num_fields(res);
		char** fields = (char**)malloc(sizeof(char*) * num_fields);
		if (fields != NULL) {
			for (int i = 0; i < num_fields; i++) {
				fields[i] = (char*)malloc(sizeof(char) * MAX_FIELD_LENGTH);
				if (fields[i] != NULL) {
					memset(fields[i], 0, sizeof(char) * MAX_FIELD_LENGTH);
					sprintf(fields[i], "%s", mysql_fetch_field(res)->name);
				}
			}

			lua_newtable(L);

			lua_Integer k = 0;
			MYSQL_ROW row;
			while (row = mysql_fetch_row(res)) { // 取完这行，再取下一行;
				lua_pushinteger(L, k + 1);

				// 创建一个子表
				lua_newtable(L);
				for (int i = 0; i < num_fields; ++i) {
					lua_pushstring(L, fields[i]);
					lua_pushstring(L, row[i]);
					lua_settable(L, -3);
				}

				//主表位于-3
				lua_settable(L, -3);

				k++;
			}

			for (int i = 0; i < num_fields; i++) {
				free(fields[i]);
			}
			free(fields);
			mysql_free_result(res);
		}
		else {
			lua_pushnil(L);
		}
	}

	return 1;
}


static int
lmysql_close(struct lua_State* L) {
	MYSQL* context = (MYSQL*)lua_touserdata(L, 1);
	mysql_close(context);
	return 0;
}

COBWEB_CMOD_API int
luaopen_mysql(lua_State* L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{"create", lmysql_create},
		{"query", lmysql_query},
		{"noquery", lmysql_noquery},
		{"close", lmysql_close},
		{NULL, NULL}
	};
	luaL_newlib(L, l);
	return 1;
}

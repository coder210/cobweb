#include "def.h"
#include "platform/platform.h"
#include <memory.h>
#include <malloc.h>

extern "C" {
#include "lua/lapi.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

#define CC_MESSAGE_SIZE 1024

static int
ltcp_create(struct lua_State* L) {
	int sockfd = ccp_tcp_create();
	lua_pushinteger(L, sockfd);
	return 1;
}

static int
ltcp_connect(struct lua_State* L) {
	lua_Integer sockfd = lua_tointeger(L, 1);
	const char* ip = luaL_checkstring(L, 2);
	lua_Integer port = luaL_checkinteger(L, 3);
	if (ccp_tcp_connect(sockfd, ip, port)) {
		lua_pushboolean(L, true);
		lua_pushstring(L, "success");
	}
	else {
		lua_pushboolean(L, false);
		lua_pushstring(L, "fail");
	}
	return 2;
}

static int
ltcp_bind(struct lua_State* L) {
	int sockfd = luaL_checkinteger(L, 1);
	const char* ip = luaL_checkstring(L, 2);
	int port = luaL_checkinteger(L, 3);
	if (ccp_socket_bind(sockfd, ip, port)) {
		lua_pushboolean(L, true);
	}
	else {
		lua_pushboolean(L, false);
	}
	return 1;
}

static int
ltcp_listen(struct lua_State* L) {
	int sockfd = luaL_checkinteger(L, 1);
	if (ccp_tcp_listen(sockfd)) {
		lua_pushboolean(L, true);
		lua_pushstring(L, "success");
	}
	else {
		lua_pushboolean(L, false);
		lua_pushstring(L, "fail");
	}
	return 2;
}

static int
ltcp_accept(struct lua_State* L) {
	int sockfd = luaL_checkinteger(L, 1);
	struct sockaddr_t sockaddr = ccp_tcp_accept(sockfd);
	lua_pushinteger(L, sockaddr.sockfd);
	lua_pushstring(L, sockaddr.ip);
	lua_pushinteger(L, sockaddr.port);
	return 3;
}

static int
ltcp_nonblock(struct lua_State* L) {
	int sockfd = luaL_checkinteger(L, 1);
	if (ccp_socket_nonblock(sockfd)) {
		lua_pushboolean(L, true);
	}
	else {
		lua_pushboolean(L, false);
	}
	return 1;
}

static int
ltcp_recv(struct lua_State* L) {
	int sockfd = luaL_checkinteger(L, 1);
	unsigned char buf[CC_MESSAGE_SIZE];
	memset(buf, 0, CC_MESSAGE_SIZE);
	int len = ccp_tcp_recv(sockfd, (char*)buf);
	if (len > 0) {
		lua_pushinteger(L, len);
		lua_pushlstring(L, (char*)buf, len);
	}
	else {
		lua_pushinteger(L, len);
		lua_pushnil(L);
	}

	return 2;
}


static int
ltcp_send(struct lua_State* L) {
	int sockfd = luaL_checkinteger(L, 1);
	size_t len = 0;
	const char* ptr = luaL_checklstring(L, 2, &len);
	int n = ccp_tcp_send(sockfd, ptr, len);
	lua_pushinteger(L, n);
	return 1;
}


static int
ltcp_close(struct lua_State* L) {
	int sockfd = luaL_checkinteger(L, 1);
	if (ccp_socket_close(sockfd)) {
		lua_pushboolean(L, true);
		lua_pushstring(L, "success");
	}
	else {
		lua_pushboolean(L, false);
		lua_pushstring(L, "fail");
	}
	return 2;
}


MOD_API int
luaopen_tcp(lua_State* L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{"create", ltcp_create},
		{"connect", ltcp_connect},
		{"nonblock", ltcp_nonblock},
		{"bind", ltcp_bind},
		{"listen", ltcp_listen},
		{"accept", ltcp_accept},
		{"send", ltcp_send},
		{"recv", ltcp_recv},
		{"close", ltcp_close},
		{NULL, NULL}
	};
	luaL_newlib(L, l);
	return 1;
}


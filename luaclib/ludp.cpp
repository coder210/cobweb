#include <string.h>
#include "def.h"
#include "platform/platform.h"
extern "C" {
#include "lua/lapi.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}

#define CC_MAX_IP 128 
#define CC_MESSAGE_SIZE 1024


static int
ludp_create(struct lua_State* L) {
	int sockfd = platform_udp_create();
	lua_pushinteger(L, sockfd);
	return 1;
}

static int
ludp_bind(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	const char* ip = luaL_checkstring(L, 2);
	int port = (int)luaL_checkinteger(L, 3);
	if (platform_socket_bind(sockfd, ip, port)) {
		lua_pushboolean(L, true);
	}
	else {
		lua_pushboolean(L, false);
	}

	return 1;
}

static int
ludp_nonblock(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	bool result = platform_socket_nonblock(sockfd);
	lua_pushboolean(L, result);
	return 1;
}

static int
ludp_recvfrom(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	char buffer[CC_MESSAGE_SIZE] = { 0 };
	char ip[CC_MAX_IP] = { 0 };
	int port = 0;
	int n = platform_udp_recv(sockfd, buffer, ip, &port);
	lua_pushboolean(L, n != -1);
	lua_newtable(L);
	if (n != -1) {
		lua_pushstring(L, "buffer");
		lua_pushlstring(L, buffer, n);
		lua_settable(L, -3);
	}
	else {
		lua_pushstring(L, "buffer");
		lua_pushstring(L, strerror(n));
		lua_settable(L, -3);
	}

	lua_pushstring(L, "len");
	lua_pushinteger(L, n);
	lua_settable(L, -3);

	lua_pushstring(L, "ip");
	lua_pushstring(L, ip);
	lua_settable(L, -3);

	lua_pushstring(L, "port");
	lua_pushinteger(L, port);
	lua_settable(L, -3);

	return 2;
}

static int
ludp_recvfromx(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	char buffer[CC_MESSAGE_SIZE] = { 0 };
	char ip[CC_MAX_IP] = { 0 };
	int port = 0;
	int n = platform_udp_recv(sockfd, buffer, ip, &port);
	lua_pushboolean(L, n != -1);
	lua_newtable(L);
	if (n != -1) {
		lua_pushstring(L, "buffer");
		lua_pushlstring(L, buffer, n);
		lua_settable(L, -3);
	}
	else {
		lua_pushstring(L, "buffer");
		lua_pushstring(L, strerror(n));
		lua_settable(L, -3);
	}

	lua_pushstring(L, "len");
	lua_pushinteger(L, n);
	lua_settable(L, -3);

	lua_pushstring(L, "ip");
	lua_pushstring(L, ip);
	lua_settable(L, -3);

	lua_pushstring(L, "port");
	lua_pushinteger(L, port);
	lua_settable(L, -3);

	return 2;
}

static int
ludp_sendto(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	const char* buffer = luaL_checkstring(L, 2);
	int len = (int)luaL_checkinteger(L, 3);
	const char* ip = luaL_checkstring(L, 4);
	int port = (int)luaL_checkinteger(L, 5);
	int n = platform_udp_send(sockfd, buffer, len, ip, port);
	lua_pushinteger(L, n);
	return 1;
}


static int
ludp_sendtox(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	size_t len = 0;
	const char* buffer = luaL_checklstring(L, 2, &len);
	const char* ip = luaL_checkstring(L, 3);
	int port = (int)luaL_checkinteger(L, 4);
	int n = platform_udp_send(sockfd, buffer, len, ip, port);
	lua_pushinteger(L, n);
	lua_pushinteger(L, n);
	return 1;
}


static int
ludp_close(struct lua_State* L) {
	int sockfd = (int)luaL_checkinteger(L, 1);
	if (platform_socket_close(sockfd) == 0) {
		lua_pushboolean(L, true);
	}
	else {
		lua_pushboolean(L, false);
	}
	return 1;
}


COBWEB_CMOD_API int
luaopen_udp(lua_State* L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{"create", ludp_create},
		{"bind", ludp_bind},
		{"nonblock", ludp_nonblock},
		{"recvfrom", ludp_recvfrom},
		{"recvfromx", ludp_recvfromx},
		{"sendto", ludp_sendto},
		{"sendtox", ludp_sendtox},
		{"close", ludp_close},
		{NULL, NULL}
	};
	luaL_newlib(L, l);
	return 1;
}


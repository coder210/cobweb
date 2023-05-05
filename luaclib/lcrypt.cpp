#include "def.h"
extern "C" {
#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
#include "crypt/md5.h"
#include "crypt/base64.h"
#include "crypt/aes_util.h"
#include "crypt/des.h"
}


static int
lmd5(struct lua_State* L) {
	const char* input = luaL_checklstring(L, 1, NULL);
	char value[16] = { 0 };
	md5(input, strlen(input), value);
	char result[128] = { 0 };
	int i;
	for (i = 0; i < 16; i++) {
		char buf[4] = { 0 };
		sprintf(buf, "%02x", (unsigned char)value[i]);
		strcat(result, buf);
	}
	lua_pushstring(L, result);
	return 1;
}


static int
lb64_encode(struct lua_State* L) {
	size_t size = 0;
	const char* input = luaL_checklstring(L, 1, &size);
	char* result = b64_encode((const unsigned char*)input, size);
	lua_pushstring(L, result);
	free(result);
	return 1;
}

static int
lb64_decode(struct lua_State* L) {
	size_t size = 0;
	const char* input = luaL_checklstring(L, 1, &size);
	size_t de_size = 0;
	const char* result = (const char*)b64_decode_ex(input, size, &de_size);
	lua_pushlstring(L, result, de_size);
	lua_pushinteger(L, de_size);
	free((void*)result);
	return 2;
}

static int
laes_encrypt256(struct lua_State* L) {
	int key_len;
	const char* KEY = luaL_checklstring(L, 1, (size_t*)&key_len);
	const char* IV = luaL_checkstring(L, 2);
	int input_len;
	const char* input = luaL_checklstring(L, 3, (size_t*)&input_len);
	if (key_len == 32) {
		size_t out_len = 0;
		unsigned char* out = aes_encrypt256x((const unsigned char*)input, input_len, &out_len, (const unsigned char*)KEY, (const unsigned char*)IV);
		lua_pushlstring(L, (const char*)out, out_len);
		lua_pushinteger(L, out_len);
		free(out);
	}
	else {
		lua_pushnil(L);
		lua_pushinteger(L, 0);
	}

	return 2;
}

static int
laes_decrypt256(struct lua_State* L) {
	int key_len;
	const char* KEY = luaL_checklstring(L, 1, (size_t*)&key_len);
	const char* IV = luaL_checkstring(L, 2);

	int input_len;
	const char* input = luaL_checklstring(L, 3, (size_t*)&input_len);
	if (key_len == 32) {
		int out_len = 0;
		unsigned char* out = aes_decrypt256x((const unsigned char*)input, input_len, (size_t*)&out_len, (const unsigned char*)KEY, (const unsigned char*)IV);
		lua_pushlstring(L, (const char*)out, out_len);
		lua_pushinteger(L, out_len);
		free(out);
	}
	else {
		lua_pushnil(L);
		lua_pushinteger(L, 0);
	}

	return 2;
}


static int
laes_encrypt128(struct lua_State* L) {
	int key_len;
	const char* KEY = luaL_checklstring(L, 1, (size_t*)&key_len);
	const char* IV = luaL_checkstring(L, 2);
	int input_len;
	const char* input = luaL_checklstring(L, 3, (size_t*)&input_len);
	if (key_len == 16) {
		size_t out_len = 0;
		unsigned char* out = aes_encrypt128x((const unsigned char*)input, input_len, &out_len, (const unsigned char*)KEY, (const unsigned char*)IV);
		lua_pushlstring(L, (const char*)out, out_len);
		lua_pushinteger(L, out_len);
		free(out);
	}
	else {
		lua_pushnil(L);
		lua_pushinteger(L, 0);
	}
	return 2;
}

static int
laes_decrypt128(struct lua_State* L) {
	int key_len;
	const char* KEY = luaL_checklstring(L, 1, (size_t*)&key_len);
	const char* IV = luaL_checkstring(L, 2);

	int input_len;
	const char* input = luaL_checklstring(L, 3, (size_t*)&input_len);
	if (key_len == 16) {
		int out_len = 0;
		unsigned char* out = aes_decrypt128x((const unsigned char*)input, input_len, (size_t*)&out_len, (const unsigned char*)KEY, (const unsigned char*)IV);
		lua_pushlstring(L, (const char*)out, out_len);
		lua_pushinteger(L, out_len);
		free(out);
	}
	else {
		lua_pushnil(L);
		lua_pushinteger(L, 0);
	}

	return 2;
}

static void
des_key(lua_State* L, uint32_t SK[32]) {
	size_t keysz = 0;
	const void* key = luaL_checklstring(L, 1, &keysz);
	if (keysz != 8) {
		luaL_error(L, "Invalid key size %d, need 8 bytes", (int)keysz);
	}
	des_main_ks(SK, (uint8_t*)key);
}

static int
ldes_encrypt(lua_State* L) {
	uint32_t SK[32];
	des_key(L, SK);

	size_t textsz = 0;
	const uint8_t* text = (const uint8_t*)luaL_checklstring(L, 2, &textsz);
	size_t chunksz = (textsz + 8) & ~7;
	uint8_t tmp[SMALL_CHUNK];
	uint8_t* buffer = tmp;
	if (chunksz > SMALL_CHUNK) {
		buffer = (uint8_t*)lua_newuserdata(L, chunksz);
	}
	int i;
	for (i = 0; i < (int)textsz - 7; i += 8) {
		des_crypt(SK, text + i, buffer + i);
	}
	int bytes = textsz - i;
	uint8_t tail[8];
	int j;
	for (j = 0; j < 8; j++) {
		if (j < bytes) {
			tail[j] = text[i + j];
		}
		else if (j == bytes) {
			tail[j] = 0x80;
		}
		else {
			tail[j] = 0;
		}
	}
	des_crypt(SK, tail, buffer + i);
	lua_pushlstring(L, (const char*)buffer, chunksz);

	return 1;
}


static int
ldes_decrypt(lua_State* L) {
	uint32_t ESK[32];
	des_key(L, ESK);
	uint32_t SK[32];
	size_t i;
	for (i = 0; i < 32; i += 2) {
		SK[i] = ESK[30 - i];
		SK[i + 1] = ESK[31 - i];
	}
	size_t textsz = 0;
	const uint8_t* text = (const uint8_t*)luaL_checklstring(L, 2, &textsz);
	if ((textsz & 7) || textsz == 0) {
		return luaL_error(L, "Invalid des crypt text length %d", (int)textsz);
	}
	uint8_t tmp[SMALL_CHUNK];
	uint8_t* buffer = tmp;
	if (textsz > SMALL_CHUNK) {
		buffer = (uint8_t*)lua_newuserdata(L, textsz);
	}
	for (i = 0; i < textsz; i += 8) {
		des_crypt(SK, text + i, buffer + i);
	}
	int padding = 1;
	for (i = textsz - 1; i >= textsz - 8; i--) {
		if (buffer[i] == 0) {
			padding++;
		}
		else if (buffer[i] == 0x80) {
			break;
		}
		else {
			return luaL_error(L, "Invalid des crypt text");
		}
	}
	if (padding > 8) {
		return luaL_error(L, "Invalid des crypt text");
	}
	lua_pushlstring(L, (const char*)buffer, textsz - padding);
	return 1;
}


MOD_API int
luaopen_crypt(lua_State* L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{"md5", lmd5},
		{"b64en", lb64_encode},
		{"b64de", lb64_decode},
		{"aes256_en", laes_encrypt256},
		{"aes256_de", laes_decrypt256},
		{"aes128_en", laes_encrypt128},
		{"aes128_de", laes_decrypt128},
		{"des_en", ldes_encrypt},
		{"des_de", ldes_decrypt},
		{NULL, NULL}
	};
	luaL_newlib(L, l);
	return 1;
}
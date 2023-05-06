#include <assert.h>
#include <memory.h>
#include <malloc.h>
#include <stdint.h>
#include "def.h"
#include "cobweb.h"
extern "C" {
#include "lua/lapi.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"
}


#define TYPE_NIL 0
#define TYPE_BOOLEAN 1
// hibits 0 false 1 true
#define TYPE_NUMBER 2
// hibits 0 : 0 , 1: byte, 2:word, 4: dword, 6: qword, 8 : double
#define TYPE_NUMBER_ZERO 0
#define TYPE_NUMBER_BYTE 1
#define TYPE_NUMBER_WORD 2
#define TYPE_NUMBER_DWORD 4
#define TYPE_NUMBER_QWORD 6
#define TYPE_NUMBER_REAL 8

#define TYPE_USERDATA 3
#define TYPE_SHORT_STRING 4
// hibits 0~31 : len
#define TYPE_LONG_STRING 5
#define TYPE_TABLE 6

#define MAX_COOKIE 32
#define COMBINE_TYPE(t,v) ((t) | (v) << 3)

#define BLOCK_SIZE 128
#define MAX_DEPTH 32

struct block {
	struct block* next;
	char buffer[BLOCK_SIZE];
};

struct write_block {
	struct block* head;
	struct block* current;
	int len;
	int ptr;
};

struct read_block {
	char* buffer;
	int len;
	int ptr;
};

inline static struct block*
blk_alloc(void) {
	struct block* b = (struct block*)malloc(sizeof(struct block));
	b->next = NULL;
	return b;
}

inline static void
wb_push(struct write_block* b, const void* buf, int sz) {
	const char* buffer = (const char*)buf;
	if (b->ptr == BLOCK_SIZE) {
	_again:
		b->current = b->current->next = blk_alloc();
		b->ptr = 0;
	}
	if (b->ptr <= BLOCK_SIZE - sz) {
		memcpy(b->current->buffer + b->ptr, buffer, sz);
		b->ptr += sz;
		b->len += sz;
	}
	else {
		int copy = BLOCK_SIZE - b->ptr;
		memcpy(b->current->buffer + b->ptr, buffer, copy);
		buffer += copy;
		b->len += copy;
		sz -= copy;
		goto _again;
	}
}

static void
wb_init(struct write_block* wb, struct block* b) {
	wb->head = b;
	assert(b->next == NULL);
	wb->len = 0;
	wb->current = wb->head;
	wb->ptr = 0;
}

static void
wb_free(struct write_block* wb) {
	struct block* blk = wb->head;
	blk = blk->next;	// the first block is on stack
	while (blk) {
		struct block* next = blk->next;
		free(blk);
		blk = next;
	}
	wb->head = NULL;
	wb->current = NULL;
	wb->ptr = 0;
	wb->len = 0;
}

static void
rball_init(struct read_block* rb, char* buffer, int size) {
	rb->buffer = buffer;
	rb->len = size;
	rb->ptr = 0;
}

static void*
rb_read(struct read_block* rb, int sz) {
	if (rb->len < sz) {
		return NULL;
	}

	int ptr = rb->ptr;
	rb->ptr += sz;
	rb->len -= sz;
	return rb->buffer + ptr;
}

static inline void
wb_nil(struct write_block* wb) {
	uint8_t n = TYPE_NIL;
	wb_push(wb, &n, 1);
}

static inline void
wb_boolean(struct write_block* wb, int boolean) {
	uint8_t n = COMBINE_TYPE(TYPE_BOOLEAN, boolean ? 1 : 0);
	wb_push(wb, &n, 1);
}

static inline void
wb_integer(struct write_block* wb, lua_Integer v) {
	int type = TYPE_NUMBER;
	if (v == 0) {
		uint8_t n = COMBINE_TYPE(type, TYPE_NUMBER_ZERO);
		wb_push(wb, &n, 1);
	}
	else if (v != (int32_t)v) {
		uint8_t n = COMBINE_TYPE(type, TYPE_NUMBER_QWORD);
		int64_t v64 = v;
		wb_push(wb, &n, 1);
		wb_push(wb, &v64, sizeof(v64));
	}
	else if (v < 0) {
		int32_t v32 = (int32_t)v;
		uint8_t n = COMBINE_TYPE(type, TYPE_NUMBER_DWORD);
		wb_push(wb, &n, 1);
		wb_push(wb, &v32, sizeof(v32));
	}
	else if (v < 0x100) {
		uint8_t n = COMBINE_TYPE(type, TYPE_NUMBER_BYTE);
		wb_push(wb, &n, 1);
		uint8_t byte = (uint8_t)v;
		wb_push(wb, &byte, sizeof(byte));
	}
	else if (v < 0x10000) {
		uint8_t n = COMBINE_TYPE(type, TYPE_NUMBER_WORD);
		wb_push(wb, &n, 1);
		uint16_t word = (uint16_t)v;
		wb_push(wb, &word, sizeof(word));
	}
	else {
		uint8_t n = COMBINE_TYPE(type, TYPE_NUMBER_DWORD);
		wb_push(wb, &n, 1);
		uint32_t v32 = (uint32_t)v;
		wb_push(wb, &v32, sizeof(v32));
	}
}

static inline void
wb_real(struct write_block* wb, double v) {
	uint8_t n = COMBINE_TYPE(TYPE_NUMBER, TYPE_NUMBER_REAL);
	wb_push(wb, &n, 1);
	wb_push(wb, &v, sizeof(v));
}

static inline void
wb_pointer(struct write_block* wb, void* v) {
	uint8_t n = TYPE_USERDATA;
	wb_push(wb, &n, 1);
	wb_push(wb, &v, sizeof(v));
}

static inline void
wb_string(struct write_block* wb, const char* str, int len) {
	if (len < MAX_COOKIE) {
		uint8_t n = COMBINE_TYPE(TYPE_SHORT_STRING, len);
		wb_push(wb, &n, 1);
		if (len > 0) {
			wb_push(wb, str, len);
		}
	}
	else {
		uint8_t n;
		if (len < 0x10000) {
			n = COMBINE_TYPE(TYPE_LONG_STRING, 2);
			wb_push(wb, &n, 1);
			uint16_t x = (uint16_t)len;
			wb_push(wb, &x, 2);
		}
		else {
			n = COMBINE_TYPE(TYPE_LONG_STRING, 4);
			wb_push(wb, &n, 1);
			uint32_t x = (uint32_t)len;
			wb_push(wb, &x, 4);
		}
		wb_push(wb, str, len);
	}
}

static void pack_one(lua_State* L, struct write_block* b, int index, int depth);

static int
wb_table_array(lua_State* L, struct write_block* wb, int index, int depth) {
	int array_size = lua_rawlen(L, index);
	if (array_size >= MAX_COOKIE - 1) {
		uint8_t n = COMBINE_TYPE(TYPE_TABLE, MAX_COOKIE - 1);
		wb_push(wb, &n, 1);
		wb_integer(wb, array_size);
	}
	else {
		uint8_t n = COMBINE_TYPE(TYPE_TABLE, array_size);
		wb_push(wb, &n, 1);
	}

	int i;
	for (i = 1; i <= array_size; i++) {
		lua_rawgeti(L, index, i);
		pack_one(L, wb, -1, depth);
		lua_pop(L, 1);
	}

	return array_size;
}

static void
wb_table_hash(lua_State* L, struct write_block* wb, int index, int depth, int array_size) {
	lua_pushnil(L);
	while (lua_next(L, index) != 0) {
		if (lua_type(L, -2) == LUA_TNUMBER) {
			if (lua_isinteger(L, -2)) {
				lua_Integer x = lua_tointeger(L, -2);
				if (x > 0 && x <= array_size) {
					lua_pop(L, 1);
					continue;
				}
			}
		}
		pack_one(L, wb, -2, depth);
		pack_one(L, wb, -1, depth);
		lua_pop(L, 1);
	}
	wb_nil(wb);
}

static void
wb_table_metapairs(lua_State* L, struct write_block* wb, int index, int depth) {
	uint8_t n = COMBINE_TYPE(TYPE_TABLE, 0);
	wb_push(wb, &n, 1);
	lua_pushvalue(L, index);
	lua_call(L, 1, 3);
	for (;;) {
		lua_pushvalue(L, -2);
		lua_pushvalue(L, -2);
		lua_copy(L, -5, -3);
		lua_call(L, 2, 2);
		int type = lua_type(L, -2);
		if (type == LUA_TNIL) {
			lua_pop(L, 4);
			break;
		}
		pack_one(L, wb, -2, depth);
		pack_one(L, wb, -1, depth);
		lua_pop(L, 1);
	}
	wb_nil(wb);
}

static void
wb_table(lua_State* L, struct write_block* wb, int index, int depth) {
	luaL_checkstack(L, LUA_MINSTACK, NULL);
	if (index < 0) {
		index = lua_gettop(L) + index + 1;
	}
	if (luaL_getmetafield(L, index, "__pairs") != LUA_TNIL) {
		wb_table_metapairs(L, wb, index, depth);
	}
	else {
		int array_size = wb_table_array(L, wb, index, depth);
		wb_table_hash(L, wb, index, depth, array_size);
	}
}

static void
pack_one(lua_State* L, struct write_block* b, int index, int depth) {
	if (depth > MAX_DEPTH) {
		wb_free(b);
		luaL_error(L, "serialize can't pack too depth table");
	}
	int type = lua_type(L, index);
	switch (type) {
	case LUA_TNIL:
		wb_nil(b);
		break;
	case LUA_TNUMBER: {
		if (lua_isinteger(L, index)) {
			lua_Integer x = lua_tointeger(L, index);
			wb_integer(b, x);
		}
		else {
			lua_Number n = lua_tonumber(L, index);
			wb_real(b, n);
		}
		break;
	}
	case LUA_TBOOLEAN:
		wb_boolean(b, lua_toboolean(L, index));
		break;
	case LUA_TSTRING: {
		size_t sz = 0;
		const char* str = lua_tolstring(L, index, &sz);
		wb_string(b, str, (int)sz);
		break;
	}
	case LUA_TLIGHTUSERDATA:
		wb_pointer(b, lua_touserdata(L, index));
		break;
	case LUA_TTABLE: {
		if (index < 0) {
			index = lua_gettop(L) + index + 1;
		}
		wb_table(L, b, index, depth + 1);
		break;
	}
	default:
		wb_free(b);
		luaL_error(L, "Unsupport type %s to serialize", lua_typename(L, type));
	}
}

static void
pack_from(lua_State* L, struct write_block* b, int from) {
	int n = lua_gettop(L) - from;
	int i;
	for (i = 1; i <= n; i++) {
		pack_one(L, b, from + i, 0);
	}
}

static inline void
invalid_stream_line(lua_State* L, struct read_block* rb, int line) {
	int len = rb->len;
	luaL_error(L, "Invalid serialize stream %d (line:%d)", len, line);
}

#define invalid_stream(L,rb) invalid_stream_line(L,rb,__LINE__)

static lua_Integer
get_integer(lua_State* L, struct read_block* rb, int cookie) {
	switch (cookie) {
	case TYPE_NUMBER_ZERO:
		return 0;
	case TYPE_NUMBER_BYTE: {
		uint8_t n;
		uint8_t* pn = (uint8_t*)rb_read(rb, sizeof(n));
		if (pn == NULL)
			invalid_stream(L, rb);
		n = *pn;
		return n;
	}
	case TYPE_NUMBER_WORD: {
		uint16_t n;
		uint16_t* pn = (uint16_t*)rb_read(rb, sizeof(n));
		if (pn == NULL)
			invalid_stream(L, rb);
		memcpy(&n, pn, sizeof(n));
		return n;
	}
	case TYPE_NUMBER_DWORD: {
		int32_t n;
		int32_t* pn = (int32_t*)rb_read(rb, sizeof(n));
		if (pn == NULL)
			invalid_stream(L, rb);
		memcpy(&n, pn, sizeof(n));
		return n;
	}
	case TYPE_NUMBER_QWORD: {
		int64_t n;
		int64_t* pn = (int64_t*)rb_read(rb, sizeof(n));
		if (pn == NULL)
			invalid_stream(L, rb);
		memcpy(&n, pn, sizeof(n));
		return n;
	}
	default:
		invalid_stream(L, rb);
		return 0;
	}
}

static double
get_real(lua_State* L, struct read_block* rb) {
	double n;
	double* pn = (double*)rb_read(rb, sizeof(n));
	if (pn == NULL)
		invalid_stream(L, rb);
	memcpy(&n, pn, sizeof(n));
	return n;
}

static void*
get_pointer(lua_State* L, struct read_block* rb) {
	void* userdata = 0;
	void** v = (void**)rb_read(rb, sizeof(userdata));
	if (v == NULL) {
		invalid_stream(L, rb);
	}
	memcpy(&userdata, v, sizeof(userdata));
	return userdata;
}

static void
get_buffer(lua_State* L, struct read_block* rb, int len) {
	char* p = (char*)rb_read(rb, len);
	if (p == NULL) {
		invalid_stream(L, rb);
	}
	lua_pushlstring(L, p, len);
}

static void unpack_one(lua_State* L, struct read_block* rb);

static void
unpack_table(lua_State* L, struct read_block* rb, int array_size) {
	if (array_size == MAX_COOKIE - 1) {
		uint8_t type;
		uint8_t* t = (uint8_t*)rb_read(rb, sizeof(type));
		if (t == NULL) {
			invalid_stream(L, rb);
		}
		type = *t;
		int cookie = type >> 3;
		if ((type & 7) != TYPE_NUMBER || cookie == TYPE_NUMBER_REAL) {
			invalid_stream(L, rb);
		}
		array_size = (int)get_integer(L, rb, cookie);
	}
	luaL_checkstack(L, LUA_MINSTACK, NULL);
	lua_createtable(L, array_size, 0);
	int i;
	for (i = 1; i <= array_size; i++) {
		unpack_one(L, rb);
		lua_rawseti(L, -2, i);
	}
	for (;;) {
		unpack_one(L, rb);
		if (lua_isnil(L, -1)) {
			lua_pop(L, 1);
			return;
		}
		unpack_one(L, rb);
		lua_rawset(L, -3);
	}
}

static void
push_value(lua_State* L, struct read_block* rb, int type, int cookie) {
	switch (type) {
	case TYPE_NIL:
		lua_pushnil(L);
		break;
	case TYPE_BOOLEAN:
		lua_pushboolean(L, cookie);
		break;
	case TYPE_NUMBER:
		if (cookie == TYPE_NUMBER_REAL) {
			lua_pushnumber(L, get_real(L, rb));
		}
		else {
			lua_pushinteger(L, get_integer(L, rb, cookie));
		}
		break;
	case TYPE_USERDATA:
		lua_pushlightuserdata(L, get_pointer(L, rb));
		break;
	case TYPE_SHORT_STRING:
		get_buffer(L, rb, cookie);
		break;
	case TYPE_LONG_STRING: {
		if (cookie == 2) {
			uint16_t* plen = (uint16_t*)rb_read(rb, 2);
			if (plen == NULL) {
				invalid_stream(L, rb);
			}
			uint16_t n;
			memcpy(&n, plen, sizeof(n));
			get_buffer(L, rb, n);
		}
		else {
			if (cookie != 4) {
				invalid_stream(L, rb);
			}
			uint32_t* plen = (uint32_t*)rb_read(rb, 4);
			if (plen == NULL) {
				invalid_stream(L, rb);
			}
			uint32_t n;
			memcpy(&n, plen, sizeof(n));
			get_buffer(L, rb, n);
		}
		break;
	}
	case TYPE_TABLE: {
		unpack_table(L, rb, cookie);
		break;
	}
	default: {
		invalid_stream(L, rb);
		break;
	}
	}
}

static void
unpack_one(lua_State* L, struct read_block* rb) {
	uint8_t type;
	uint8_t* t = (uint8_t*)rb_read(rb, sizeof(type));
	if (t == NULL) {
		invalid_stream(L, rb);
	}
	type = *t;
	push_value(L, rb, type & 0x7, type >> 3);
}

static void
seri(lua_State* L, struct block* b, int len) {
	uint8_t* buffer = (uint8_t*)malloc(len);
	uint8_t* ptr = buffer;
	int sz = len;
	while (len > 0) {
		if (len >= BLOCK_SIZE) {
			memcpy(ptr, b->buffer, BLOCK_SIZE);
			ptr += BLOCK_SIZE;
			len -= BLOCK_SIZE;
			b = b->next;
		}
		else {
			memcpy(ptr, b->buffer, len);
			break;
		}
	}

	lua_pushlightuserdata(L, buffer);
	lua_pushinteger(L, sz);
}

static int
luaseri_unpack(lua_State* L) {
	if (lua_isnoneornil(L, 1)) {
		return 0;
	}
	void* buffer;
	int len;
	if (lua_type(L, 1) == LUA_TSTRING) {
		size_t sz;
		buffer = (void*)lua_tolstring(L, 1, &sz);
		len = (int)sz;
	}
	else {
		buffer = lua_touserdata(L, 1);
		len = (int)luaL_checkinteger(L, 2);
	}
	if (len == 0) {
		return 0;
	}
	if (buffer == NULL) {
		return luaL_error(L, "deserialize null pointer");
	}

	lua_settop(L, 1);
	struct read_block rb;
	rball_init(&rb, (char*)buffer, len);

	int i;
	for (i = 0;; i++) {
		if (i % 8 == 7) {
			luaL_checkstack(L, LUA_MINSTACK, NULL);
		}
		uint8_t type = 0;
		uint8_t* t = (uint8_t*)rb_read(&rb, sizeof(type));
		if (t == NULL)
			break;
		type = *t;
		push_value(L, &rb, type & 0x7, type >> 3);
	}

	// Need not free buffer

	return lua_gettop(L) - 1;
}

static int
luaseri_pack(lua_State* L) {
	struct block temp;
	temp.next = NULL;
	struct write_block wb;
	wb_init(&wb, &temp);
	pack_from(L, &wb, 0);
	assert(wb.head == &temp);
	seri(L, &temp, wb.len);

	wb_free(&wb);

	return 2;
}


static int
lversion(struct lua_State* L) {
	lua_pushstring(L, "v1.0.0");
	return 1;
}

static int
traceback(lua_State* L) {
	const char* msg = lua_tostring(L, 1);
	if (msg)
		luaL_traceback(L, L, msg, 1);
	else {
		lua_pushliteral(L, "(no error message)");
	}
	return 1;
}

static int
_cb(struct context_t* ctx, void* ud, int type,
	int session, uint32_t source,
	const void* msg, size_t sz) {
	lua_State* L = (lua_State*)ud;
	int trace = 1;
	int r;
	int top = lua_gettop(L);
	if (top == 0) {
		lua_pushcfunction(L, traceback);  //错误处理的函数
		lua_rawgetp(L, LUA_REGISTRYINDEX, _cb);  //把表里的回调函数取出来
	}
	else {
		assert(top == 2);
	}
	lua_pushvalue(L, 2); //回调函数入栈

	lua_pushinteger(L, type);  //参数type入栈
	lua_pushlightuserdata(L, (void*)msg); //参数msg入栈
	lua_pushinteger(L, sz); //参数sz，消息长度，入栈
	lua_pushinteger(L, session);  //参数session入栈
	lua_pushinteger(L, source); //参数source入栈

	r = lua_pcall(L, 5, 0, trace); //调用lua的回调函数，也就是cobweb.dispatch_message

	if (r == LUA_OK) {
		return 0;
	}
	const char* self = ctx->command(ctx, "REG", NULL);
	switch (r) {
	case LUA_ERRRUN:
		ctx->log(ctx, "lua call [%x to %s : %d msgsz = %d] error : %s", source, self, session, sz, lua_tostring(L, -1));
		break;
	case LUA_ERRMEM:
		ctx->log(ctx, "lua memory error : [%x to %s : %d]", source, self, session);
		break;
	case LUA_ERRERR:
		ctx->log(ctx, "lua error in error : [%x to %s : %d]", source, self, session);
		break;
	case LUA_ERRGCMM:
		ctx->log(ctx, "lua gc error : [%x to %s : %d]", source, self, session);
		break;
	};

	lua_pop(L, 1);

	return 0;
}

static int
forward_cb(struct context_t* ctx, void* ud, int type, int session, uint32_t source, const void* msg, size_t sz) {
	_cb(ctx, ud, type, session, source, msg, sz);
	// don't delete msg in forward mode.
	return 1;
}

// 设置回调函数
static int
lcallback(lua_State* L) {
	struct context_t* ctx = (struct context_t*)lua_touserdata(L, lua_upvalueindex(1));
	int forward = lua_toboolean(L, 2);
	luaL_checktype(L, 1, LUA_TFUNCTION);
	lua_settop(L, 1);
	lua_rawsetp(L, LUA_REGISTRYINDEX, _cb);

	lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_MAINTHREAD);
	lua_State* gL = lua_tothread(L, -1);

	if (forward) {
		ctx->callback(ctx, gL, forward_cb);
	}
	else {
		ctx->callback(ctx, gL, _cb);
	}

	return 0;
}

// 命令函数
static int
lcommand(lua_State* L) {
	struct context_t* ctx = (struct context_t*)lua_touserdata(L, lua_upvalueindex(1));
	const char* cmd = luaL_checkstring(L, 1);
	const char* result;
	const char* parm = NULL;
	if (lua_gettop(L) == 2) {
		parm = luaL_checkstring(L, 2);
	}
	result = ctx->command(ctx, cmd, parm);
	if (result) {
		lua_pushstring(L, result);
		return 1;
	}
	return 0;
}

// 调用命令
static int
lintcommand(lua_State* L) {
	struct context_t* ctx = (struct context_t*)lua_touserdata(L, lua_upvalueindex(1));
	const char* cmd = luaL_checkstring(L, 1);
	const char* result;
	const char* parm = NULL;
	char tmp[64] = { 0 };	// for integer parm
	if (lua_gettop(L) == 2) {
		int32_t n = (int32_t)luaL_checkinteger(L, 2);
		sprintf(tmp, "%d", n);
		parm = tmp;
	}

	result = ctx->command(ctx, cmd, parm);
	if (result) {
		lua_Integer r = strtoll(result, NULL, 0);
		lua_pushinteger(L, r);
		return 1;
	}
	return 0;
}

static int
lgenid(lua_State* L) {
	struct context_t* ctx = (struct context_t*)lua_touserdata(L, lua_upvalueindex(1));
	int session = ctx->send(ctx, 0, 0, PTYPE_TAG_ALLOCSESSION, 0, NULL, 0);
	lua_pushinteger(L, session);
	return 1;
}

static const char*
get_dest_string(lua_State* L, int index) {
	const char* dest_string = lua_tostring(L, index);
	if (dest_string == NULL) {
		luaL_error(L, "dest address type (%s) must be a string or number.", lua_typename(L, lua_type(L, index)));
	}
	return dest_string;
}

static int
lsend(lua_State* L) {
	struct context_t* ctx = (struct context_t*)lua_touserdata(L, lua_upvalueindex(1));
	uint32_t dest = (uint32_t)lua_tointeger(L, 1);
	const char* dest_string = NULL;
	if (dest == 0) {
		if (lua_type(L, 1) == LUA_TNUMBER) {
			return luaL_error(L, "Invalid service address 0");
		}
		dest_string = get_dest_string(L, 1);
	}

	int type = (int)luaL_checkinteger(L, 2);
	int session = 0;
	if (lua_isnil(L, 3)) {
		type |= PTYPE_TAG_ALLOCSESSION;
	}
	else {
		session = (int)luaL_checkinteger(L, 3);
	}

	int mtype = lua_type(L, 4);
	switch (mtype) {
	case LUA_TSTRING: {
		size_t len = 0;
		void* msg = (void*)lua_tolstring(L, 4, &len);
		if (len == 0) {
			msg = NULL;
		}
		if (dest_string) {
			session = ctx->sendname(ctx, 0, dest_string, type, session, msg, len);
		}
		else {
			session = ctx->send(ctx, 0, dest, type, session, msg, len);
		}
		break;
	}
	case LUA_TLIGHTUSERDATA: {
		void* msg = lua_touserdata(L, 4);
		int size = (int)luaL_checkinteger(L, 5);
		if (dest_string) {
			session = ctx->sendname(ctx, 0, dest_string, type | PTYPE_TAG_DONTCOPY, session, msg, size);
		}
		else {
			session = ctx->send(ctx, 0, dest, type | PTYPE_TAG_DONTCOPY, session, msg, size);
		}
		break;
	}
	default:
		luaL_error(L, "cobweb.send invalid param %s", lua_typename(L, lua_type(L, 4)));
	}
	if (session < 0) {
		// send to invalid address
		// todo: maybe throw an error would be better
		return 0;
	}
	lua_pushinteger(L, session);
	return 1;
}

static int
lredirect(lua_State* L) {
	struct context_t* ctx = (struct context_t*)lua_touserdata(L, lua_upvalueindex(1));
	uint32_t dest = (uint32_t)lua_tointeger(L, 1);
	const char* dest_string = NULL;
	if (dest == 0) {
		dest_string = get_dest_string(L, 1);
	}
	uint32_t source = (uint32_t)luaL_checkinteger(L, 2);
	int type = (int)luaL_checkinteger(L, 3);
	int session = (int)luaL_checkinteger(L, 4);

	int mtype = lua_type(L, 5);
	switch (mtype) {
	case LUA_TSTRING: {
		size_t len = 0;
		void* msg = (void*)lua_tolstring(L, 5, &len);
		if (len == 0) {
			msg = NULL;
		}
		if (dest_string) {
			session = ctx->sendname(ctx, source, dest_string, type, session, msg, len);
		}
		else {
			session = ctx->send(ctx, source, dest, type, session, msg, len);
		}
		break;
	}
	case LUA_TLIGHTUSERDATA: {
		void* msg = lua_touserdata(L, 5);
		int size = (int)luaL_checkinteger(L, 6);
		if (dest_string) {
			session = ctx->sendname(ctx, source, dest_string, type | PTYPE_TAG_DONTCOPY, session, msg, size);
		}
		else {
			session = ctx->send(ctx, source, dest, type | PTYPE_TAG_DONTCOPY, session, msg, size);
		}
		break;
	}
	default:
		luaL_error(L, "cobweb.redirect invalid param %s", lua_typename(L, mtype));
	}
	return 0;
}

static int
lerror(lua_State* L) {
	struct context_t* ctx = (struct context_t*)lua_touserdata(L, lua_upvalueindex(1));
	int n = lua_gettop(L);
	if (n <= 1) {
		lua_settop(L, 1);
		const char* s = luaL_tolstring(L, 1, NULL);
		ctx->error(ctx, "%s", s);
		return 0;
	}
	luaL_Buffer b;
	luaL_buffinit(L, &b);
	int i;
	for (i = 1; i <= n; i++) {
		luaL_tolstring(L, i, NULL);
		luaL_addvalue(&b);
		if (i < n) {
			luaL_addchar(&b, ' ');
		}
	}
	luaL_pushresult(&b);
	ctx->error(ctx, "%s", lua_tostring(L, -1));
	return 0;
}

static int
llog(lua_State* L) {
	struct context_t* ctx = (struct context_t*)lua_touserdata(L, lua_upvalueindex(1));
	int n = lua_gettop(L);
	if (n <= 1) {
		lua_settop(L, 1);
		const char* s = luaL_tolstring(L, 1, NULL);
		ctx->log(ctx, "%s", s);
		return 0;
	}
	luaL_Buffer b;
	luaL_buffinit(L, &b);
	int i;
	for (i = 1; i <= n; i++) {
		luaL_tolstring(L, i, NULL);
		luaL_addvalue(&b);
		if (i < n) {
			luaL_addchar(&b, ' ');
		}
	}
	luaL_pushresult(&b);
	ctx->log(ctx, "%s", lua_tostring(L, -1));
	return 0;
}

static int
ltostring(lua_State* L) {
	if (lua_isnoneornil(L, 1)) {
		return 0;
	}
	char* msg = (char*)lua_touserdata(L, 1);
	int sz = (int)luaL_checkinteger(L, 2);
	lua_pushlstring(L, msg, sz);
	return 1;
}

static int
lharbor(lua_State* L) {
	struct context_t* context = (struct context_t*)lua_touserdata(L, lua_upvalueindex(1));
	uint32_t handle = (uint32_t)luaL_checkinteger(L, 1);
	int harbor = 0;
	//int remote = cobweb_isremote(context, handle, &harbor);
	int remote = 0;
	lua_pushinteger(L, harbor);
	lua_pushboolean(L, remote);
	return 2;
}

static int
lpackstring(lua_State* L) {
	luaseri_pack(L);
	char* str = (char*)lua_touserdata(L, -2);
	int sz = (int)lua_tointeger(L, -1);
	lua_pushlstring(L, str, sz);
	free(str);
	return 1;
}

static int
ltrash(lua_State* L) {
	int t = lua_type(L, 1);
	switch (t) {
	case LUA_TSTRING: {
		break;
	}
	case LUA_TLIGHTUSERDATA: {
		void* msg = lua_touserdata(L, 1);
		luaL_checkinteger(L, 2);
		free(msg);
		break;
	}
	default:
		luaL_error(L, "spider.trash invalid param %s", lua_typename(L, t));
	}

	return 0;
}

static int
ltointeger(lua_State* L) {
	int n = (int)luaL_checknumber(L, 1);
	lua_pushinteger(L, n);
	return 1;
}

static int
lsocket_send(lua_State* L) {
	struct context_t* ctx = (struct context_t*)lua_touserdata(L, lua_upvalueindex(1));
	int fd = (int)luaL_checkinteger(L, 1);
	const char* ptr = luaL_checkstring(L, 2);
	int sz = (int)luaL_checkinteger(L, 3);
	int n = ctx->sserver_send(fd, ptr, sz);
	lua_pushinteger(L, n);
	return 1;
}

static int
lsocket_unpack(lua_State* L) {
	struct socket_message_t* socket_message = (struct socket_message_t*)lua_touserdata(L, 1);
	lua_pushinteger(L, socket_message->type);
	lua_pushinteger(L, socket_message->fd);
	if (socket_message->type == SOCKET_TYPE_DATA) {
		lua_pushlstring(L, socket_message->buffer, socket_message->sz);
		lua_pushinteger(L, socket_message->sz);
	}
	else {
		lua_pushstring(L, "");
		lua_pushinteger(L, 0);
	}

	return 4;
}

COBWEB_CMOD_API int
luaopen_lcobweb(lua_State* L) {
	luaL_checkversion(L);
	luaL_Reg l[] = {
		{"version", lversion},
		{"callback", lcallback},
		{"command", lcommand},
		{"intcommand", lintcommand},
		{"genid", lgenid},
		{"pack", luaseri_pack },
		{"unpack", luaseri_unpack },
		{"send", lsend},
		{"redirect", lredirect},
		{"error", lerror},
		{"log", llog},
		{"packstring", lpackstring},
		{"tostring", ltostring},
		{"harbor", lharbor},
		{"trash", ltrash},
		{"tointeger", ltointeger},
		{"socket_send", lsocket_send},
		{"socket_unpack", lsocket_unpack},
		{NULL, NULL}
	};
	luaL_newlibtable(L, l);

	lua_getfield(L, LUA_REGISTRYINDEX, "__this");
	struct context_t* ctx = (struct context_t*)lua_touserdata(L, -1);
	if (ctx == NULL) {
		printf("Init __this first");
		return 0;
	}

	luaL_setfuncs(L, l, 1);

	return 1;
}



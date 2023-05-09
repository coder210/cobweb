/**************************************************
Copyright: 2021-2022, lanchong.xyz/Ltd.
File name: cobweb_server.cpp
Description: 此服务器采用ctx模型,也是主要操作对象
Author: ydlc
Version: 1.0
Date: 2021.12.12
History:
****************************************************/
#include "cobweb.h"
#include <assert.h>
#include <stdarg.h>
#include <queue>
#include <mutex>

struct context_data_t {
	void* instance;
	struct module_t* mod;
	void* cb_ud;
	context_callback cb;
	std::queue<message_t> mq;
	std::mutex mq_mutex;
	FILE* logfile;
	char result[32];
	uint32_t handle;
	int session_id;
	bool exit;
	bool endless;
};

static uint32_t
_tohandle(struct context_t* ctx, const char* param) {
	uint32_t handle = 0;
	if (param[0] == ':') {
		handle = strtoul(param + 1, NULL, 16);
	}
	else if (param[0] == '.') {
		handle = cobweb_handle_findname(param + 1);
	}
	else {
		cobweb_context_error(ctx, "Can't convert %s to handle", param);
	}

	return handle;
}


/* 消息分发 */
void
_dispatch_message(struct context_t* ctx, struct message_t& msg) {
	if (ctx->_data->cb == nullptr) {
		cobweb_free(msg.data);
		return;
	}
	size_t sz = msg.sz;
	if (ctx->_data->logfile) {
		cobweb_context_log_output(ctx->_data->logfile, msg.source, msg.type, msg.session, msg.data, sz);
	}
	if (!ctx->_data->cb(ctx, ctx->_data->cb_ud, msg.type, msg.session, msg.source, msg.data, sz)) {
		cobweb_free(msg.data);
	}
}

bool
cobweb_dispatch_message(struct monitor_t* sm, struct context_t* ctx) {
	if (!ctx->_data->exit) {
		struct message_t msg = { 0 };
		bool is_empty = false;

		ctx->_data->mq_mutex.lock();
		is_empty = ctx->_data->mq.empty();
		if (!is_empty) {
			msg = ctx->_data->mq.front();
			ctx->_data->mq.pop();
		}
		ctx->_data->mq_mutex.unlock();

		if (!is_empty) {
			cobweb_monitor_trigger(sm, msg.source, ctx->_data->handle);
			_dispatch_message(ctx, msg);
			cobweb_monitor_trigger(sm, 0, 0);
		}
		return true;
	}
	else {
		cobweb_context_release(ctx);
		return false;
	}
}

void
cobweb_context_callback(struct context_t* ctx,
	void* ud, context_callback cb) {
	ctx->_data->cb_ud = ud;
	ctx->_data->cb = cb;
}

uint32_t
cobweb_context_queryname(struct context_t* ctx, const char* name) {
	switch (name[0]) {
	case ':':
		return strtoul(name + 1, NULL, 16);
	case '.':
		return cobweb_handle_findname(name + 1);
	}
	platform_error("Don't support query global name %s", name);
	return 0;
}

static void
id_to_hex(char* str, uint32_t id) {
	int i;
	static char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	str[0] = ':';
	for (i = 0; i < 8; i++) {
		str[i + 1] = hex[(id >> ((7 - i) * 4)) & 0xf];
	}
	str[9] = '\0';
}

static void
handle_exit(struct context_t* ctx, uint32_t handle) {
	if (handle == 0) {
		handle = ctx->_data->handle;
		ctx->_data->exit = true;
		cobweb_context_log(ctx, "KILL self");
	}
	else {
		cobweb_context_log(ctx, "KILL :%0x", handle);
	}
}

// 命令函数
struct command_func {
	const char* name;
	const char* (*func)(struct context_t* ctx, const char* param);
};

static const char*
cmd_timeout(struct context_t* ctx, const char* param) {
	char* session_ptr = NULL;
	int ti = strtol(param, &session_ptr, 10);
	int session = cobweb_context_newsession(ctx);
	cobweb_add_trigger(ctx->_data->handle, session, ti, NULL, 0);
	memset(ctx->_data->result, 0, 32);
	sprintf(ctx->_data->result, "%d", session);
	return ctx->_data->result;
}

static const char*
cmd_reg(struct context_t* ctx, const char* param) {
	if (param == NULL || param[0] == '\0') {
		memset(ctx->_data->result, 0, 32);
		sprintf(ctx->_data->result, ":%x", ctx->_data->handle);
		return ctx->_data->result;
	}
	else if (param[0] == '.') {
		return cobweb_handle_namehandle(ctx->_data->handle, param + 1);
	}
	else {
		platform_error("Can't register global name %s in C", param);
		return NULL;
	}
}

static const char*
cmd_query(struct context_t* ctx, const char* param) {
	if (param[0] == '.') {
		uint32_t handle = cobweb_handle_findname(param + 1);
		if (handle) {
			memset(ctx->_data->result, 0, 32);
			sprintf(ctx->_data->result, ":%x", handle);
			return ctx->_data->result;
		}
	}
	return NULL;
}

static const char*
cmd_name(struct context_t* ctx, const char* param) {
	size_t size = strlen(param);
	char* name = (char*)cobweb_malloca(size + 1);
	char* handle = (char*)cobweb_malloca(size + 1);
	if (name == NULL || handle == NULL) {
		return NULL;
	}
	sscanf(param, "%s %s", name, handle);
	if (handle[0] != ':') {
		return NULL;
	}
	uint32_t handle_id = strtoul(handle + 1, NULL, 16);
	if (handle_id == 0) {
		return NULL;
	}
	if (name[0] == '.') {
		const char* result = cobweb_handle_namehandle(handle_id, name + 1);
		return result;
	}
	else {
		platform_error("Can't set global name %s in C", name);
		return NULL;
	}
}

static const char*
cmd_exit(struct context_t* ctx, const char* param) {
	handle_exit(ctx, 0);
	return NULL;
}

static const char*
cmd_kill(struct context_t* ctx, const char* param) {
	uint32_t handle = _tohandle(ctx, param);
	if (handle) {
		handle_exit(ctx, handle);
	}
	return NULL;
}


static const char*
cmd_launch(struct context_t* ctx, const char* param) {
	char* tmp = cobweb_strdup(param);
	char* args = tmp;
	char* mod = cobweb_strsep(&args, " \t\r\n");
	args = cobweb_strsep(&args, "\r\n");
	struct context_t* inst = cobweb_context_new(mod, args);
	if (inst == nullptr) {
		cobweb_free(tmp);
		return NULL;
	}
	else {
		cobweb_free(tmp);
		id_to_hex(inst->_data->result, inst->_data->handle);
		return inst->_data->result;
	}
}

static const char*
cmd_getstring(struct context_t* ctx, const char* param) {
	return cobweb_getenv(param);
}

static const char*
cmd_setstring(struct context_t* ctx, const char* param) {
	char* key = cobweb_strdup(param);
	int i;
	for (i = 0; param[i] != ' ' && param[i]; i++) {
		key[i] = param[i];
	}
	if (param[i] == '\0') {
		return NULL;
	}

	key[i] = '\0';
	param += i + 1;

	cobweb_setenv(key, param);
	return NULL;
}

static const char*
cmd_starttime(struct context_t* ctx, const char* param) {
	memset(ctx->_data->result, 0, 32);
	return ctx->_data->result;
}

static const char*
cmd_endless(struct context_t* ctx, const char* param) {
	if (ctx->_data->endless) {
		memset(ctx->_data->result, 0, 32);
		strcpy(ctx->_data->result, "1");
		ctx->_data->endless = false;
		return ctx->_data->result;
	}
	return NULL;
}

static const char*
cmd_abort(struct context_t* ctx, const char* param) {

	return NULL;
}

static const char*
cmd_mqlen(struct context_t* ctx, const char* param) {
	int len = ctx->_data->mq.size();
	memset(ctx->_data->result, 0, 32);
	sprintf(ctx->_data->result, "%d", len);
	return ctx->_data->result;
}

static const char*
cmd_logon(struct context_t* ctx, const char* param) {
	uint32_t handle = _tohandle(ctx, param);
	if (handle == 0) {
		return NULL;
	}
	struct context_t* dest_ctx = cobweb_handle_find(handle);
	if (dest_ctx != NULL && dest_ctx->_data->logfile == NULL) {
		const char* logpath = "";
		if (logpath != NULL) {
			size_t sz = strlen(logpath);
			char* tmp = (char*)cobweb_malloca(sz + 16);
			sprintf(tmp, "%s/%08x.log", logpath, handle);
			dest_ctx->_data->logfile = fopen(tmp, "ab");
		}
	}
	return NULL;
}

static const char*
cmd_logoff(struct context_t* ctx, const char* param) {
	uint32_t handle = _tohandle(ctx, param);
	if (handle == 0) {
		return NULL;
	}
	struct context_t* dest_ctx = cobweb_handle_find(handle);
	if (dest_ctx != NULL && dest_ctx->_data->logfile != NULL) {
		platform_error("Close log file :%08x", handle);
		fprintf(dest_ctx->_data->logfile, "close time: %u\n", (uint32_t)cobweb_timestamp());
		fclose(dest_ctx->_data->logfile);
		dest_ctx->_data->logfile = NULL;
	}
	return NULL;
}

static const char*
cmd_signal(struct context_t* ctx, const char* param) {
	uint32_t handle = _tohandle(ctx, param);
	if (handle == 0) {
		return NULL;
	}
	struct context_t* dest_ctx = cobweb_handle_find(handle);
	if (dest_ctx == NULL) {
		return NULL;
	}
	param = strchr(param, ' ');
	int sig = 0;
	if (param) {
		sig = strtol(param, NULL, 0);
	}
	// NOTICE: the signal function should be thread safe.
	cobweb_module_instance_signal(ctx->_data->mod, ctx->_data->instance, sig);
	return NULL;
}


static struct command_func cmd_funcs[] = {
	{ "TIMEOUT", cmd_timeout },
	{ "REG", cmd_reg },
	{ "QUERY", cmd_query },
	{ "NAME", cmd_name },
	{ "EXIT", cmd_exit },
	{ "KILL", cmd_kill },
	{ "LAUNCH", cmd_launch },
	{ "GETSTRING", cmd_getstring },
	{ "SETSTRING", cmd_setstring },
	{ "STARTTIME", cmd_starttime },
	{ "ENDLESS", cmd_endless },
	{ "ABORT", cmd_abort },
	{ "MQLEN", cmd_mqlen },
	{ "LOGON", cmd_logon },
	{ "LOGOFF", cmd_logoff },
	{ "SIGNAL", cmd_signal },
	{ NULL, NULL },
};

const char*
cobweb_context_command(struct context_t* ctx, const char* cmd, const char* param) {
	struct command_func* method = &cmd_funcs[0];
	while (method->name) {
		if (strcmp(cmd, method->name) == 0) {
			return method->func(ctx, param);
		}
		++method;
	}

	return NULL;
}

int
cobweb_context_push(uint32_t handle, struct message_t& msg) {
	struct context_t* ctx = cobweb_handle_find(handle);
	if (ctx == nullptr) {
		return -1;
	}
	ctx->_data->mq_mutex.lock();
	ctx->_data->mq.push(msg);
	ctx->_data->mq_mutex.unlock();
	cobweb_monitor_wakeup();
	return 0;
}

static void
_filter_args(struct context_t* ctx, int* type, int* session, void** data, size_t sz) {
	int needcopy = !(*type & PTYPE_TAG_DONTCOPY);
	int allocsession = *type & PTYPE_TAG_ALLOCSESSION;
	*type &= 0xff; // 去掉tag

	if (allocsession) {
		assert(*session == 0);
		*session = cobweb_context_newsession(ctx);
	}

	if (needcopy && *data) {
		char* msg = (char*)cobweb_malloc(sz + 1);
		if (msg != NULL) {
			memcpy(msg, *data, sz);
			msg[sz] = 0;
		}
		*data = msg;
	}
}

/* 发送消息 */
int
cobweb_context_send(struct context_t* ctx, uint32_t source, uint32_t dest, int type, int session, void* data, size_t sz) {
	if (sz > COBWEB_MAX_SIZE) {
		cobweb_context_log(ctx, "The message to %x is too large", dest);
		if ((type & PTYPE_TAG_DONTCOPY) == PTYPE_TAG_DONTCOPY) {
			cobweb_free(data);
		}
		return -1;
	}

	_filter_args(ctx, &type, &session, (void**)&data, sz);

	if (source == 0) {
		source = ctx->_data->handle;
	}

	if (dest == 0) {
		return session;
	}

	struct message_t msg;
	msg.type = type;
	msg.source = source;
	msg.session = session;
	msg.data = data;
	msg.sz = sz;
	if (cobweb_context_push(dest, msg)) {
		return -1;
	}
	return session;
}

int
cobweb_context_sendname(struct context_t* ctx, uint32_t source, const char* addr, int type, int session, void* data, size_t sz) {
	if (source == 0) {
		source = ctx->_data->handle;
	}
	uint32_t des = 0;

	if (addr[0] == ':') {
		des = strtoul(addr + 1, NULL, 16);
	}
	else if (addr[0] == '.') {
		des = cobweb_handle_findname(addr + 1);
	}
	else {
		//struct remote_message* rmsg = cobweb_malloc(sizeof(*rmsg));
		//copy_name(rmsg->dest.name, addr);
		//rmsg->dest.handle = 0;
		//rmsg->message = data;
		//rmsg->sz = sz;
		//harbor_send(rmsg, source, session);
		return session;
	}

	return cobweb_context_send(ctx, source, des, type, session, data, sz);
}

void
cobweb_context_endless(struct context_t* ctx) {
	if (ctx == NULL) {
		return;
	}
	ctx->_data->endless = true;
}

struct context_t*
	cobweb_context_new(const char* name, const char* param) {
	struct module_t* mod = cobweb_module_query(name);
	if (mod == nullptr) {
		return nullptr;
	}
	void* inst = cobweb_module_instance_create(mod);
	if (inst == nullptr) {
		return nullptr;
	}

	struct context_data_t* private_data = new context_data_t();
	if (private_data == nullptr) {
		return nullptr;
	}

	struct context_t* ctx = new context_t();
	if (ctx == nullptr) {
		delete private_data;
		return nullptr;
	}

	ctx->_data = private_data;
	ctx->_data->mod = mod;
	ctx->_data->instance = inst;
	ctx->_data->cb = nullptr;
	ctx->_data->cb_ud = nullptr;
	ctx->_data->session_id = 0;
	ctx->_data->logfile = nullptr;
	ctx->_data->endless = false;
	ctx->_data->exit = false;
	ctx->_data->handle = cobweb_handle_register(ctx);

	/* public method */
	ctx->send = cobweb_context_send;
	ctx->sendname = cobweb_context_sendname;
	ctx->callback = cobweb_context_callback;
	ctx->command = cobweb_context_command;
	ctx->log = cobweb_context_log;
	ctx->error = cobweb_context_error;

	int r = cobweb_module_instance_init(mod, inst, ctx, param);
	if (r == 0) {
		cobweb_cq_eq(ctx);
		return ctx;
	}
	else {
		return nullptr;
	}
}

void
cobweb_context_release(struct context_t* ctx) {
	if (ctx != nullptr) {
		if (ctx->_data->logfile) {
			fclose(ctx->_data->logfile);
		}
		cobweb_module_instance_release(ctx->_data->mod, ctx->_data->instance);
		cobweb_handle_retire(ctx->_data->handle);
		delete ctx->_data;
		delete ctx;
		ctx = nullptr;
	}
}

int
cobweb_context_newsession(struct context_t* ctx) {
	int session = ++ctx->_data->session_id;
	if (session <= 0) {
		ctx->_data->session_id = 1;
		return 1;
	}
	return session;
}

uint32_t
cobweb_context_handle(struct context_t* ctx) {
	return ctx->_data->handle;
}

void
cobweb_context_logfile(struct context_t* ctx, int type, void* data, size_t sz) {
	uint32_t logger = cobweb_handle_findname("logger");
	if (logger == 0) {
		return;
	}
	struct message_t format;
	if (ctx == nullptr) {
		format.source = 0;
	}
	else {
		format.source = cobweb_context_handle(ctx);
	}
	format.session = 0;
	format.data = data;
	format.sz = sz;
	format.type = type;
	cobweb_context_push(logger, format);
}

void
cobweb_context_log(struct context_t* ctx, const char* format, ...) {
	char tmp[COBWEB_MESSAGE_SIZE];
	char* data = nullptr;

	va_list ap;

	va_start(ap, format);
	int len = vsnprintf(tmp, COBWEB_MESSAGE_SIZE, format, ap);
	va_end(ap);
	if (len >= 0 && len < COBWEB_MESSAGE_SIZE) {
		data = cobweb_strdup(tmp);
	}
	else {
		int max_size = COBWEB_MESSAGE_SIZE;
		for (;;) {
			max_size *= 2;
			data = (char*)cobweb_malloc(max_size);
			va_start(ap, format);
			len = vsnprintf(data, max_size, format, ap);
			va_end(ap);
			if (len < max_size) {
				break;
			}
			cobweb_free(data);
		}
	}
	if (len < 0) {
		cobweb_free(data);
		platform_red_print("vsnprintf error :");
		return;
	}

	cobweb_context_logfile(ctx, PTYPE_TEXT, data, len);
}

void
cobweb_context_error(struct context_t* ctx, const char* format, ...) {
	char tmp[COBWEB_MESSAGE_SIZE];
	char* data = nullptr;

	va_list ap;

	va_start(ap, format);
	int len = vsnprintf(tmp, COBWEB_MESSAGE_SIZE, format, ap);
	va_end(ap);
	if (len >= 0 && len < COBWEB_MESSAGE_SIZE) {
		data = cobweb_strdup(tmp);
	}
	else {
		int max_size = COBWEB_MESSAGE_SIZE;
		for (;;) {
			max_size *= 2;
			data = (char*)cobweb_malloc(max_size);
			va_start(ap, format);
			len = vsnprintf(data, max_size, format, ap);
			va_end(ap);
			if (len < max_size) {
				break;
			}
			cobweb_free(data);
		}
	}
	if (len < 0) {
		cobweb_free(data);
		platform_red_print("vsnprintf error :");
		return;
	}

	cobweb_context_logfile(ctx, PTYPE_ERROR, data, len);
}


static void
log_blob(FILE* f, void* buffer, size_t sz) {
	size_t i;
	uint8_t* buf = (uint8_t*)buffer;
	for (i = 0; i != sz; i++) {
		fprintf(f, "%02x", buf[i]);
	}
}

void
cobweb_context_log_output(FILE* f, uint32_t source, int type, int session, void* buffer, size_t sz) {
	if (type == PTYPE_SOCKET) {
		//log_socket(f, buffer, sz);
	}
	else {
		uint32_t ti = (uint32_t)cobweb_timestamp();
		fprintf(f, ":%08x %d %d %u ", source, type, session, ti);
		log_blob(f, buffer, sz);
		fprintf(f, "\n");
		fflush(f);
	}
}

/**************************************************
Copyright: 2021-2022, lanchong.xyz/Ltd.
File name: cobweb_server.c
Description: 此服务器采用ctx模型,也是主要操作对象
Author: ydlc
Version: 1.0
Date: 2021.12.12
History:
****************************************************/
#include "cobweb.h"
#include <assert.h>
#include <stdarg.h>

struct context_data_t {
	void* instance; //指向模块内部的数据结构
	struct module_t* mod;//指向module
	void* cb_ud;//用户自定义数据，回调函数cb的参数
	context_callback cb;//函数指针，指向模块_cb回调函数
	struct mq_t* mq;//指向包含本handle ID的消息队列
	FILE* logfile;
	char result[32];//主要用来保存cmd_xxx函数的操作结果
	uint32_t handle;//本ctx的handle ID
	int session_id; // session id
	uint64_t starttime; // 开始时间
	int state; // 状态
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
_dispatch_message(struct context_t* ctx, struct message_t* msg) {
	//如果服务都没提供回调
	if (ctx->_data->cb == NULL) {
		cobweb_free(msg->data);
	}
	else {
		size_t sz = msg->sz;
		if (ctx->_data->logfile) {
			cobweb_context_log_output(ctx->_data->logfile, msg->source, msg->type, msg->session, msg->data, sz);
		}
		// 调用回调函数处理数据
		if (!ctx->_data->cb(ctx, ctx->_data->cb_ud, msg->type, msg->session, msg->source, msg->data, sz)) {
			cobweb_free(msg->data);
		}
	}

	cobweb_free(msg);
}



/* 动作 */
bool
cobweb_context_action(struct context_t* ctx, struct monitor_t* sm) {
	bool ret = true;
	if (ctx->_data->state == STATE_DISPATCH) {
		struct message_t* pmsg = cobweb_mq_dq(ctx->_data->mq);
		if (pmsg != NULL) {
			/* 触发monitor，monitor线程会检查是不是进入死循环 */
			cobweb_monitor_trigger(sm, pmsg->source, ctx->_data->handle);

			/* 处理消息(调用cb函数) */
			_dispatch_message(ctx, pmsg);

			/* 调用结束了，当destination为0的时候，不进行死循环检查 */
			cobweb_monitor_trigger(sm, 0, 0);
		}
	}
	else if (ctx->_data->state == STATE_EXIT) {
		ctx->_data->state = STATE_CLEAR;
	}
	else if (ctx->_data->state == STATE_CLEAR) {
		/* clear */
		cobweb_context_log(ctx, "CLEAR");
		cobweb_context_release(ctx);
		ret = false;
	}
	else {
		cobweb_context_log(ctx, ":%0x unknown state", ctx->_data->handle);
		ctx->_data->state = STATE_EXIT;
	}

	return ret;
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
		ctx->_data->state = STATE_EXIT;
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
	cobweb_add_wheel(ctx->_data->handle, session, ti, NULL, 0);
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
	if (inst == NULL) {
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
	sprintf(ctx->_data->result, "%llu", ctx->_data->starttime);
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
	int len = cobweb_mq_length(ctx->_data->mq);
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
	struct context_t* dest_ctx = cobweb_handle_grab(handle);
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
	struct context_t* dest_ctx = cobweb_handle_grab(handle);
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
	struct context_t* dest_ctx = cobweb_handle_grab(handle);
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
cobweb_context_push(uint32_t handle, struct message_t* message) {
	struct context_t* ctx = cobweb_handle_grab(handle);  //增加ctx引用计数
	if (ctx == NULL) {
		return -1;
	}
	cobweb_mq_eq(ctx->_data->mq, message); //消息入队完成
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

/* 发送网络消息 */
int
cobweb_context_ssend(struct socket_message_t* socket_message) {
	struct message_t* pmsg = (struct message_t*)cobweb_malloc(sizeof(struct message_t));
	if (pmsg != NULL) {
		pmsg->type = PTYPE_SOCKET;
		pmsg->source = 0;
		pmsg->session = 0;
		pmsg->data = socket_message;
		pmsg->sz = sizeof(struct socket_message_t);
		uint32_t gate = cobweb_handle_findname("gate");
		if (cobweb_context_push(gate, pmsg)) {
			cobweb_free(pmsg->data);
			cobweb_free(pmsg);
			return 0;
		}
	}

	return -1;
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

	struct message_t* pmsg = (struct message_t*)cobweb_malloc(sizeof(struct message_t));
	if (pmsg) {
		pmsg->type = type;
		pmsg->source = source;
		pmsg->session = session;
		pmsg->data = data;
		pmsg->sz = sz;
		if (cobweb_context_push(dest, pmsg)) {
			cobweb_free(pmsg->data);
			cobweb_free(pmsg);
			return -1;
		}
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
		//copy_name(rmsg->destination.name, addr);
		//rmsg->destination.handle = 0;
		//rmsg->message = data;
		//rmsg->sz = sz;
		//harbor_send(rmsg, source, session);
		return session;
	}

	return cobweb_context_send(ctx, source, des, type, session, data, sz);
}

// endless
void
cobweb_context_endless(struct context_t* ctx) {
	if (ctx == NULL) {
		return;
	}
	ctx->_data->endless = true;
}

// ctx
struct context_t*
cobweb_context_new(const char* name, const char* param) {
	struct module_t* mod = cobweb_module_query(name);
	if (mod == NULL) {
		return NULL;
	}
	void* inst = cobweb_module_instance_create(mod);
	if (inst == NULL) {
		return NULL;
	}

	struct context_data_t* private_data = (struct context_data_t*)cobweb_malloc(sizeof(struct context_data_t));
	if (private_data == NULL) {
		return NULL;
	}

	memset(private_data, 0, sizeof(struct context_data_t));

	struct context_t* ctx = (struct context_t*)cobweb_malloc(sizeof(struct context_t));
	if (ctx == NULL) {
		cobweb_free(private_data);
		return NULL;
	}

	ctx->_data = private_data;
	ctx->_data->mod = mod;
	ctx->_data->instance = inst;
	ctx->_data->cb = NULL;
	ctx->_data->cb_ud = NULL;
	ctx->_data->session_id = 0;
	ctx->_data->logfile = NULL;
	ctx->_data->state = STATE_WAIT;
	ctx->_data->endless = false;
	ctx->_data->handle = cobweb_handle_register(ctx);
	ctx->_data->starttime = cobweb_timestamp();

	/* public method */
	ctx->sserver_send = cobweb_sserver_send;
	ctx->send = cobweb_context_send;
	ctx->sendname = cobweb_context_sendname;
	ctx->callback = cobweb_context_callback;
	ctx->command = cobweb_context_command;
	ctx->log = cobweb_context_log;
	ctx->error = cobweb_context_error;


	struct mq_t* mq = ctx->_data->mq = cobweb_mq_create(ctx->_data->handle);
	int r = cobweb_module_instance_init(mod, inst, ctx, param);
	if (r == 0) {
		ctx->_data->state = STATE_DISPATCH;
		cobweb_globalmq_eq(mq);
		return ctx;
	}
	else {
		ctx->_data->state = STATE_EXIT;
		return NULL;
	}
}

void
cobweb_context_release(struct context_t* ctx) {
	if (ctx != NULL) {
		if (ctx->_data->logfile) {
			fclose(ctx->_data->logfile);
		}
		cobweb_module_instance_release(ctx->_data->mod, ctx->_data->instance);
		cobweb_handle_retire(ctx->_data->handle);
		cobweb_mq_release(ctx->_data->mq);
		cobweb_free(ctx);
		ctx = NULL;
	}
}

// session
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

	struct message_t* pformat = (struct message_t*)cobweb_malloc(sizeof(struct message_t));
	if (pformat != NULL) {
		if (ctx == NULL) {
			pformat->source = 0;
		}
		else {
			pformat->source = cobweb_context_handle(ctx);
		}
		pformat->session = 0;
		pformat->data = data;
		pformat->sz = sz;
		pformat->type = type;
		if (cobweb_context_push(logger, pformat)) {
			cobweb_free(pformat->data);
			cobweb_free(pformat);
		}
	}
}

void
cobweb_context_log(struct context_t* ctx, const char* format, ...) {
	char tmp[COBWEB_MESSAGE_SIZE];
	char* data = NULL;

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
	char* data = NULL;

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

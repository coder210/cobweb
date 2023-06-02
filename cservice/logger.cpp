#include "context.h"
#include <string>
#include <iostream>


struct logger {
	FILE* handle;
	std::string filename;
	std::string param;
	int close;
};


COBWEB_MOD_API struct logger*
logger_create(void) {
	logger* inst = new logger();
	if (inst != nullptr) {
		inst->handle = NULL;
		inst->close = 0;
		inst->filename = "";
	}
	return inst;
}

COBWEB_MOD_API void
logger_release(logger* inst) {
	if (inst->close) {
		fclose(inst->handle);
	}
	delete inst;
}

static void
_log(Context* ctx, logger* inst, uint32_t source, const char* msg, size_t sz) {
	std::string currtime = ctx->current_datetime("%H:%M:%S");
	fprintf(inst->handle, "%s>[:%08x] ", currtime.c_str(), source);
	fwrite(msg, sz, 1, inst->handle);
	fprintf(inst->handle, "\n");
	fflush(inst->handle);
	ctx->info("%s>[:%08x] %s", currtime.c_str(), source, msg);
}

static void
_error(Context* ctx, struct logger* inst, uint32_t source, const char* msg, size_t sz) {
	std::string currtime = ctx->current_datetime("%H:%M:%S");
	fprintf(inst->handle, "%s>[:%08x] ", currtime.c_str(), source);
	fwrite(msg, sz, 1, inst->handle);
	fprintf(inst->handle, "\n");
	fflush(inst->handle); 
	ctx->error("%s>[:%08x] %s", currtime.c_str(), source, msg);
}

static bool 
logger_cb(Context* ctx, void* ud, int type, int session, uint32_t source, const void* data, size_t sz) {
	struct logger* inst = (struct logger*)ud;
	switch (type) {
	case PTYPE_SYSTEM:
		break;
	case PTYPE_TEXT:
		_log(ctx, inst, source, (const char*)data, sz);
		break;
	case PTYPE_ERROR:
		_error(ctx, inst, source, (const char*)data, sz);
		break;
	}

	return true;
}

COBWEB_MOD_API bool
logger_init(logger* inst, Context* ctx, std::string param) {
	inst->param = param;
	inst->filename = "./" + inst->param + "/" + ctx->current_datetime("%Y%m%d") + ".log";
	inst->handle = fopen(inst->filename.c_str(), "a+");
	if (inst->handle == nullptr) {
		return false;
	}

	inst->close = false;
	ctx->callback(ctx, inst, logger_cb);
	ctx->command(ctx, "REG", ".logger");
	return true;
}

COBWEB_MOD_API void
logger_signal(logger* inst, int signal) {

}

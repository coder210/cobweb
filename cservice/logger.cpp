#include "def.h"
#include "cobweb.h"

struct logger {
	FILE* handle;
	char* filename;
	char param[COBWEB_MAX_PATH];
	int close;
};

struct datetime_t
_currtime(void) {
	return platform_datetime("%H:%M:%S");
}

COBWEB_CMOD_API struct logger*
logger_create(void) {
	struct logger* inst = (struct logger*)cobweb_malloc(sizeof(struct logger));
	if (inst != NULL) {
		inst->handle = NULL;
		inst->close = 0;
		inst->filename = NULL;
	}
	return inst;
}

COBWEB_CMOD_API void
logger_release(struct logger* inst) {
	if (inst->close) {
		fclose(inst->handle);
	}
	free(inst->filename);
	free(inst);
}

static void
_log(struct logger* inst, uint32_t source, const void* msg, size_t sz) {
	struct datetime_t currtime = _currtime();
	fprintf(inst->handle, "%s>[:%08x] ", currtime.buffer, source);
	fwrite(msg, sz, 1, inst->handle);
	fprintf(inst->handle, "\n");
	fflush(inst->handle);
	platform_log("%s>[:%08x] %s", currtime.buffer, source, msg);
}

static void
_error(struct logger* inst, uint32_t source, const void* msg, size_t sz) {
	struct datetime_t currtime = _currtime();
	fprintf(inst->handle, "%s>[:%08x] ", currtime.buffer, source);
	fwrite(msg, sz, 1, inst->handle);
	fprintf(inst->handle, "\n");
	fflush(inst->handle);
	platform_log("%s>[:%08x] %s", currtime.buffer, source, msg);
}


static void
_reset_filename(struct logger* inst) {
	memset(inst->filename, 0, sizeof(char) * COBWEB_MAX_PATH);
	sprintf(inst->filename, "./%s/%s.log", inst->param, platform_datetime("%Y%m%d").buffer);
}

static int
logger_cb(struct context_t* context,
	void* ud, int type,
	int session, uint32_t source,
	const void* msg, size_t sz) {
	struct logger* inst = (struct logger*)ud;
	switch (type) {
	case PTYPE_SYSTEM:
		if (inst->filename && sz > 0) {
			strncpy(inst->filename, (const char*)msg, sz);
			inst->filename[sz] = 0;
			inst->handle = freopen(inst->filename, "a+", inst->handle);
		}
		break;
	case PTYPE_TEXT:
		_log(inst, source, msg, sz);
		break;
	case PTYPE_ERROR:
		_error(inst, source, msg, sz);
		break;
	}

	return 0;
}

COBWEB_CMOD_API int
logger_init(struct logger* inst,
	struct context_t* ctx,
	const char* parm) {
	if (parm) {
		strcpy(inst->param, parm);
		inst->filename = (char *)cobweb_malloc(sizeof(char) * COBWEB_MAX_PATH);
		if (inst->filename != NULL) {
			_reset_filename(inst);
			inst->handle = fopen(inst->filename, "a+");
		}
		if (inst->handle == NULL) {
			return 1;
		}
		inst->close = 1;
		ctx->callback(ctx, inst, logger_cb);
		ctx->command(ctx, "REG", ".logger");
		return 0;
	}
	return 1;
}

COBWEB_CMOD_API void
logger_signal(struct logger* inst, int signal) {
	if (inst->filename) {
		_reset_filename(inst);
		inst->handle = freopen(inst->filename, "a+", inst->handle);
	}
}

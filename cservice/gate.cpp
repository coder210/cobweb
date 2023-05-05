#include "def.h"
#include "cobweb.h"
#include <stdarg.h>

#define BACKLOG 32

struct gate {
	struct context_t* ctx;
	uint32_t listen_id;
};

MOD_API struct gate*
gate_create(void) {
	struct gate* g = (struct gate*)malloc(sizeof(struct gate));
	assert(g != NULL);
	memset(g, 0, sizeof(struct gate));
	g->listen_id = 0;
	return g;
}

MOD_API void
gate_release(struct gate* g) {
	if (g != NULL) {
		free(g);
	}
}

static int
_cb(struct context_t* ctx, void* ud, int type, int session, uint32_t source, const void* msg, size_t sz) {
	struct gate* g = (struct gate*)ud;
	switch (type) {
	case PTYPE_TEXT:
		break;
	case PTYPE_CLIENT:
		if (g->listen_id == 0) {
			g->listen_id = source;
		}
		break;
	case PTYPE_SOCKET:
		if (g->listen_id != 0) {
			void* data = malloc(sz);
			if (data != NULL) {
				memcpy(data, msg, sz);
				ctx->send(ctx, source, g->listen_id, type | PTYPE_TAG_DONTCOPY, 0, data, sz);
			}
		}
		break;
	}
	return 0;
}

MOD_API int
gate_init(struct gate* g, struct context_t* ctx, char* parm) {
	g->ctx = ctx;
	ctx->callback(ctx, g, _cb);
	ctx->command(ctx, "REG", ".gate");
	return 0;
}

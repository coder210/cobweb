#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include "def.h"
#include "cobweb.h"


#define STATUS_WAIT 0
#define STATUS_HANDSHAKE 1
#define STATUS_HEADER 2
#define STATUS_CONTENT 3
#define STATUS_DOWN 4

struct slave {
	int fd;
	struct harbor_msg_queue *queue;
	int status;
	int length;
	int read;
	uint8_t size[4];
	char * recv_buffer;
};

struct harbor {
	struct skynet_context *ctx;
	int id;
	uint32_t slave;
};


COBWEB_CMOD_API struct harbor *
harbor_create(void) {
	struct harbor * h = (struct harbor*)malloc(sizeof(struct harbor));
	assert(h != NULL);
	memset(h,0,sizeof(struct harbor));
	return h;
}

COBWEB_CMOD_API void
harbor_release(struct harbor *h) {
	free(h);
}

COBWEB_CMOD_API int
harbor_init(struct harbor *h, struct skynet_context *ctx, const char * args) {
	h->ctx = ctx;
	int harbor_id = 0;
	uint32_t slave = 0;
	h->id = harbor_id;
	h->slave = slave;
	return 0;
}

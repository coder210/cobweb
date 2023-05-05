/*********************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_mq.c
Description: 消息队列
Author: ydlc
Version: 1.0
Date: 2021.4.22
History:
***********************************************/
#include "cobweb.h"


typedef void (*message_drop)(struct message_t*, void*);

struct mq_t {
	uint32_t handle;
	struct queue_t* mq;
};

struct globalmq_t {
	struct queue_t* queue;
};


static struct globalmq_t* Q = NULL;


void
cobweb_globalmq_init(void) {
	Q = (struct globalmq_t*)cobweb_malloc(sizeof(struct globalmq_t));
	if (Q != NULL) {
		Q->queue = cobweb_queue_create();
	}
}

int
cobweb_globalmq_length(void) {
	int length = 0;
	if (Q != NULL) {
		length = cobweb_queue_length(Q->queue);
	}
	return length;
}

uint32_t
cobweb_mq_handle(struct mq_t* mq) {
	if (mq != NULL) {
		return mq->handle;
	}
	else {
		return 0;
	}
}


void
cobweb_globalmq_eq(struct mq_t* mq) {
	if (Q != NULL) {
		cobweb_eq(Q->queue, mq);
	}
}

struct mq_t*
cobweb_globalmq_dq(void) {
	struct mq_t* mq = NULL;
	if (Q != NULL) {
		mq = (struct mq_t*)cobweb_dq(Q->queue);
	}
	return mq;
}

void
cobweb_globalmq_release() {
	if (Q != NULL) {
		cobweb_queue_release(Q->queue);
		cobweb_free (Q);
		Q = NULL;
	}
}

struct mq_t*
cobweb_mq_create(uint32_t handle) {
	struct mq_t* mq = (struct mq_t*)cobweb_malloc(sizeof(struct mq_t));
	if (mq != NULL) {
		memset(mq, 0, sizeof(struct mq_t));
		mq->handle = handle;
		mq->mq = cobweb_queue_create();
	}
	return mq;
}

void
cobweb_mq_release(struct mq_t* mq) {
	if (mq != NULL) {
		cobweb_queue_release(mq->mq);
		cobweb_free (mq);
	}
}

int
cobweb_mq_length(struct mq_t* mq) {
	int length = cobweb_queue_length(mq->mq);
	return length;
}

struct message_t*
cobweb_mq_dq(struct mq_t* mq) {
	struct message_t* msg = NULL;
	if (cobweb_mq_length(mq) > 0) {
		msg = (message_t*)cobweb_dq(mq->mq);
	}
	return msg;
}

void
cobweb_mq_eq(struct mq_t* mq, struct message_t* msg) {
	cobweb_eq(mq->mq, msg);
}

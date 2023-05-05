/**********************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_infra.c
Description: 基础数据结构链表,队列,实现
Author: ydlc
Version: 1.0
Date: 2021.7.2
History:
***********************************************/
#include "cobweb.h"

// node
struct node_t {
	void* data; // data
	struct node_t* next;
};

// list
struct linkedlist_t {
	struct mutex_t* mutex;
	struct node_t* head;
};


// queue
struct queue_t {
	struct mutex_t* mutex;
	struct node_t* front;
	struct node_t* rear;
	int length;
};


struct linkedlist_t*
cobweb_linkedlist_create() {
	struct linkedlist_t* list = (struct linkedlist_t*)cobweb_malloc(sizeof(struct linkedlist_t));
	if (list != NULL) {
		list->head = NULL;
		list->mutex = ccp_mutex_create();
	}
	return list;
}

void*
cobweb_linkedlist_query(struct linkedlist_t* list, linkedlist_cb f, void* param) {
	void* data = NULL;

	ccp_mutex_lock(list->mutex);

	struct node_t* itor = list->head;
	while (itor) {
		if (f(itor->data, param)) {
			data = itor->data;
			break;
		}
		itor = itor->next;
	}

	ccp_mutex_unlock(list->mutex);

	return data;
}

void
cobweb_linkedlist_each(struct linkedlist_t* list, linkedlist_cb f, void* param) {
	struct node_t* itor = list->head;

	ccp_mutex_lock(list->mutex);

	while (itor) {
		if (f(itor->data, param)) {
			break;
		}
		itor = itor->next;
	}

	ccp_mutex_unlock(list->mutex);
}

bool
cobweb_linkedlist_contain(struct linkedlist_t* list, void* data) {
	bool result = false;

	ccp_mutex_lock(list->mutex);

	struct node_t* itor = list->head;
	while (itor) {
		if (itor->data == data) {
			result = true;
			break;
		}
		itor = itor->next;
	}

	ccp_mutex_unlock(list->mutex);

	return result;
}

bool
cobweb_linkedlist_containf(struct linkedlist_t* list, linkedlist_cb f, void* param) {
	bool result = false;

	ccp_mutex_lock(list->mutex);

	struct node_t* itor = list->head;
	while (itor) {
		if (f(itor->data, param)) {
			result = true;
			break;
		}
		itor = itor->next;
	}

	ccp_mutex_unlock(list->mutex);

	return result;
}

int
cobweb_linkedlist_length(struct linkedlist_t* list) {
	int count = 0;

	ccp_mutex_lock(list->mutex);

	struct node_t* itor = list->head;
	while (itor) {
		count++;
		itor = itor->next;
	}

	ccp_mutex_unlock(list->mutex);

	return count;
}

struct node_t*
cobweb_linkedlist_insert(struct linkedlist_t* list, void* data) {
	struct node_t* item = (struct node_t*)cobweb_malloc(sizeof(struct node_t));
	if (item != NULL) {
		memset(item, 0, sizeof(struct node_t));
		item->data = data;

		ccp_mutex_lock(list->mutex);

		if (list->head == NULL) {
			list->head = item;
		}
		else {
			item->next = list->head;
			list->head = item;
		}

		ccp_mutex_unlock(list->mutex);
	}

	return item;
}

bool
cobweb_linkedlist_removef(struct linkedlist_t* list, linkedlist_cb f, void* param) {
	bool result = false;
	if (list == NULL || list->head == NULL) {
		return result;
	}

	ccp_mutex_lock(list->mutex);

	struct node_t* prev = list->head;
	struct node_t* it = list->head->next;
	while (it) {
		if (f(it->data, param)) {
			prev->next = it->next;
			result = true;
		}
		it = it->next;
	}

	if (f(list->head->data, param)) {
		list->head = list->head->next;
		result = true;
	}

	ccp_mutex_unlock(list->mutex);

	return result;
}

bool
cobweb_linkedlist_remove(struct linkedlist_t* list, void* data) {
	bool result = false;
	if (list == NULL || list->head == NULL) {
		return result;
	}

	ccp_mutex_lock(list->mutex);

	struct node_t* prev = list->head;
	struct node_t* it = list->head->next;
	while (it) {
		if (it->data == data) {
			prev->next = it->next;
			it = it->next;
			result = true;
		}
		else {
			prev = it;
			it = it->next;
		}
	}

	if (list->head->data == data) {
		list->head = list->head->next;
		result = true;
	}

	ccp_mutex_unlock(list->mutex);

	return result;
}

bool
cobweb_linkedlist_drainf(struct linkedlist_t* list, linkedlist_cb f, void* param) {
	bool result = false;
	if (list == NULL || list->head == NULL) {
		return result;
	}

	ccp_mutex_lock(list->mutex);

	struct node_t* prev = list->head;
	struct node_t* it = list->head->next;
	while (it) {
		if (f(it->data, param)) {
			struct node_t* tmp = it;
			prev->next = it->next;
			it = it->next;
			cobweb_free (tmp);
			result = true;
		}
		else {
			prev = it;
			it = it->next;
		}
	}

	if (f(list->head->data, param)) {
		struct node_t* tmp = list->head;
		list->head = list->head->next;
		cobweb_free (tmp);
		result = true;
	}

	ccp_mutex_unlock(list->mutex);

	return result;
}

bool
cobweb_linkedlist_drain(struct linkedlist_t* list, void* data) {
	bool result = false;
	if (list == NULL || list->head == NULL) {
		return result;
	}

	ccp_mutex_lock(list->mutex);

	if (list->head->data == data) {
		struct node_t* tmp = list->head;
		list->head = list->head->next;
		cobweb_free (tmp);
		result = true;
	}
	else {
		struct node_t* prev = list->head;
		struct node_t* itor = list->head->next;
		while (itor) {
			if (itor->data == data) {
				struct node_t* tmp = itor;
				prev->next = itor->next;
				cobweb_free (tmp);
				itor = NULL;
				result = true;
				break;
			}
			itor = itor->next;
		}
	}

	ccp_mutex_unlock(list->mutex);

	return result;
}

void
cobweb_linkedlist_release(struct linkedlist_t* list) {
	if (list != NULL) {
		ccp_mutex_lock(list->mutex);

		struct node_t* itor = list->head;
		while (itor) {
			struct node_t* tmp = itor;
			itor = itor->next;
			cobweb_free (tmp);
			tmp = NULL;
		}

		ccp_mutex_unlock(list->mutex);
		ccp_mutex_release(list->mutex);
		cobweb_free (list);
	}
}

struct queue_t*
cobweb_queue_create() {
	struct queue_t* queue = (struct queue_t*)cobweb_malloc(sizeof(struct queue_t));
	if (queue != NULL) {
		queue->front = NULL;
		queue->rear = NULL;
		queue->length = 0;
		queue->mutex = ccp_mutex_create();
	}
	return queue;
}

bool
cobweb_queue_contain(struct queue_t* queue, void* data) {
	bool result = false;

	ccp_mutex_lock(queue->mutex);

	struct node_t* itor = queue->front;
	while (itor != NULL) {
		if (itor->data == data) {
			result = true;
			break;
		}
		itor = itor->next;
	}

	ccp_mutex_unlock(queue->mutex);

	return result;
}

void*
cobweb_queue_first(struct queue_t* queue) {
	void* data = NULL;

	ccp_mutex_lock(queue->mutex);

	if (queue->length >= 1) {
		data = queue->front->data;
	}

	ccp_mutex_unlock(queue->mutex);

	return data;
}

void
cobweb_eq(struct queue_t* queue, void* data) {
	ccp_mutex_lock(queue->mutex);

	struct node_t* item = (struct node_t*)cobweb_malloc(sizeof(struct node_t));
	if (item != NULL) {
		memset(item, 0, sizeof(struct node_t));
		item->data = data;

		if (queue->length == 0) {
			queue->front = item;
			queue->rear = item;
		}
		else {
			queue->rear->next = item;
			queue->rear = item;
		}
		queue->length++;

	}

	ccp_mutex_unlock(queue->mutex);
}

//  enqueue and distinct queue
void
cobweb_eqd(struct queue_t* queue, void* data) {
	ccp_mutex_lock(queue->mutex);

	bool result = false;
	struct node_t* itor = queue->front;
	while (itor != NULL) {
		if (itor->data == data) {
			result = true;
			break;
		}
		itor = itor->next;
	}

	if (!result) {
		struct node_t* item = (struct node_t*)cobweb_malloc(sizeof(struct node_t));
		if (item != NULL) {
			memset(item, 0, sizeof(struct node_t));
			item->data = data;

			if (queue->length == 0) {
				queue->front = item;
				queue->rear = item;
			}
			else {
				queue->rear->next = item;
				queue->rear = item;
			}
			queue->length++;
		}
	}

	ccp_mutex_unlock(queue->mutex);
}

void*
cobweb_dq(struct queue_t* queue) {
	void* data = NULL;

	ccp_mutex_lock(queue->mutex);

	if (queue->length == 1) {
		data = queue->front->data;
		cobweb_free (queue->front);
		queue->front = NULL;
		queue->rear = NULL;
		queue->length--;
	}
	else if (queue->length > 1) {
		data = queue->front->data;
		struct node_t* tmp = queue->front;
		queue->front = queue->front->next;
		cobweb_free (tmp);
		queue->length--;
	}

	ccp_mutex_unlock(queue->mutex);

	return data;
}

int
cobweb_queue_length(struct queue_t* queue) {
	int length = 0;

	if (queue != NULL) {
		ccp_mutex_lock(queue->mutex);
		length = queue->length;
		ccp_mutex_unlock(queue->mutex);
	}

	return length;
}

void
cobweb_queue_release(struct queue_t* queue) {
	if (queue != NULL) {

		ccp_mutex_lock(queue->mutex);

		struct node_t* itor = queue->front;
		while ((itor != NULL
			|| itor != queue->rear)
			&& queue->length > 0) {
			struct node_t* tmp = itor;
			itor = itor->next;
			cobweb_free (tmp);
			tmp = NULL;
			queue->length--;
		}

		ccp_mutex_unlock(queue->mutex);
		ccp_mutex_release(queue->mutex);
		cobweb_free (queue);
	}
}


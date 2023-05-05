/***********************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_timer.c
Description: 通过时间轮算法实现的定时器
Author: ydlc
Version: 1.0
Date: 2021.5.22
History:
************************************************/
#include "cobweb.h"


#define TIMER_WHEEL_NUM 9


struct timer_wheel_t {
	uint32_t handle; // handle
	int session; // session
	int count;
	int sec;
	void* data;
	size_t sz;
	struct timer_wheel_t* prev;
	struct timer_wheel_t* next;
};

struct timer_t {
	int fd;
	int cur_index;
	struct timer_wheel_t* wheels[TIMER_WHEEL_NUM];
};


static struct timer_t* T = NULL;


void
cobweb_timer_init(void) {
	T = (struct timer_t*)cobweb_malloc(sizeof(struct timer_t));
	if (T != NULL) {
		memset(T, 0, sizeof(struct timer_t));
		T->cur_index = 0;
		T->fd = ccp_timefd_create();
	}
}

int
cobweb_add_wheel(uint32_t handle, int session, 
	int sec, void* data, size_t sz) {
	struct timer_wheel_t* wheel = (struct timer_wheel_t*)cobweb_malloc(sizeof(struct timer_wheel_t));
	if (wheel != NULL) {
		memset(wheel, 0, sizeof(struct timer_wheel_t));
		wheel->handle = handle;
		wheel->session = session;
		wheel->sec = sec;
		wheel->data = data;
		wheel->sz = sz;

		/* 计算在哪个齿上 */ 
		wheel->count = wheel->sec / TIMER_WHEEL_NUM;
		int index = (wheel->sec + T->cur_index) % TIMER_WHEEL_NUM;
		struct timer_wheel_t* head_node = T->wheels[index];
		if (head_node == NULL) {
			T->wheels[index] = wheel;
		}
		else {
			/* 尾插法 */
			struct timer_wheel_t* it = head_node;
			while (it->next) {
				it = it->next;
			}
			it->next = wheel;
			wheel->prev = it;
		}

		return wheel->session;
	}
	else {
		return -1;
	}
}

static int
_del_wheel(int index, struct timer_wheel_t* it) {
	if (it == T->wheels[index]) {
		T->wheels[index] = it->next;
	}
	else {
		if (it->prev != NULL) {
			it->prev->next = it->next;
		}
		if (it->next != NULL) {
			it->next->prev = it->prev;
		}
	}

	return 0;
}

bool
cobweb_del_wheel(int session) {
	bool is_found = false;
	for (int i = 0; i < TIMER_WHEEL_NUM; i++) {
		struct timer_wheel_t* it = T->wheels[i];
		while (it) {
			if (it->session == session) {
				_del_wheel(i, it);
				is_found = true;
				break;
			}
			else {
				it = it->next;
			}
		}

		if (is_found) {
			break;
		}
	}
	return is_found;
}


int
cobweb_timer_tick(bool(*tick)(uint32_t handle, int session, void* data, size_t sz, void* arg), void* arg) {
	int timeout = ccp_timer_timeout(T->fd);
	while (timeout-- > 0) {
		T->cur_index++;
		T->cur_index %= TIMER_WHEEL_NUM;
		struct timer_wheel_t* it = T->wheels[T->cur_index];
		while (it) {
			if (it->count <= 0) {
				struct timer_wheel_t* it_next = it->next;

				/* 移走这个节点 */
				_del_wheel(T->cur_index, it);
				if (tick != NULL
					&& tick(it->handle, it->session, it->data, it->sz, arg)) {
					cobweb_free (it->data);
					cobweb_free (it);
					it = NULL;
				}
				else {
					cobweb_free (it->data);
					cobweb_free (it);
					it = NULL;
				}

				it = it_next;
			}
			else {
				it->count--;
				it = it->next;
			}
		}
	}

	return T->cur_index;
}

void
cobweb_timer_release(void) {
	if (T != NULL) {
		int i;
		for (i = 0; i < TIMER_WHEEL_NUM; i++) {
			struct timer_wheel_t* it = T->wheels[i];
			while (it) {
				struct timer_wheel_t* tmp = it;
				it = it->next;
				cobweb_free (tmp);
				tmp = NULL;
			}
		}

		ccp_timerfd_close(T->fd);
		cobweb_free (T);
		T = NULL;
	}
}


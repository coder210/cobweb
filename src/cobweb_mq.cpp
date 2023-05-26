/***************************************************
Copyright (C): 2020-2021, lanchong.xyz/Ltd.
File name: cobweb_mq.cpp
Description: 所有的服务在这里通过handle进行模询到
Author: ydlc
Version: 1.0
Date: 2021.10.10
History:
****************************************************/

#include "mq.h"
#include <map>
#include <cassert>


bool 
MQSystem::Dequeue(MQ* mq, Message* pmsg) {
	bool ret = false;
	if (!mq->storage_.empty()) {
		mq->mutex_.lock();
		if (!mq->storage_.empty()) {
			*pmsg = mq->storage_.front();
			mq->storage_.pop();
			ret = true;
		}
		mq->mutex_.unlock();
	}
	return ret;
}

void 
MQSystem::Enqueue(MQ* mq, Message& msg) {
	mq->mutex_.lock();
	mq->storage_.push(msg);
	mq->mutex_.unlock();
}


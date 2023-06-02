#ifndef _MQ_H
#define _MQ_H
#include <mutex>
#include <queue>


struct Message {
	int type_;
	uint32_t source_;
	int session_;
	void* data_;
	size_t sz_;
};

class MQ {
public:
	std::mutex mutex_;
	std::queue<Message> storage_;
};

class MQSystem
{
public:
	static bool Dequeue(MQ* mq, Message* pmsg);
	static void Enqueue(MQ* mq, Message& msg);
};


#endif
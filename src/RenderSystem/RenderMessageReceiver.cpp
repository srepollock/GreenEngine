#include "RenderMessageReceiver.h"

RenderMessageReceiver::RenderMessageReceiver(std::vector<std::shared_ptr<Message>> *mq_p, std::mutex *mutex)
{
	_mq_p = mq_p;
	_mqMutex_p = mutex;
}

void RenderMessageReceiver::subscribeAll()
{
	this->subscribe(MESSAGE_TYPE::RenderLoadMessageType);
	this->subscribe(MESSAGE_TYPE::RenderLoadSingleMessageType);
	this->subscribe(MESSAGE_TYPE::RenderDrawMessageType);
	this->subscribe(MESSAGE_TYPE::RenderDrawOverlayMessageType);
	this->subscribe(MESSAGE_TYPE::RenderUnloadMessageType);
}

bool RenderMessageReceiver::messageHandler(std::shared_ptr<Message> message)
{
	_mqMutex_p->lock();
	_mq_p->push_back(message);
	_mqMutex_p->unlock();

	return false;
}
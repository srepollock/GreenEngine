#include "RenderInputMessageReceiver.h"

RenderInputMessageReceiver::RenderInputMessageReceiver(std::vector<std::shared_ptr<Message>> *mq_p, std::mutex *mutex_p)
{
	_mq_p = mq_p;
	_mqMutex_p = mutex_p;
}

void RenderInputMessageReceiver::subscribeAll()
{
	this->subscribe(MESSAGE_TYPE::InputMessageType);
}

bool RenderInputMessageReceiver::messageHandler(std::shared_ptr<Message> message)
{
	_mqMutex_p->lock();
	_mq_p->push_back(message);
	_mqMutex_p->unlock();

	return false;
}
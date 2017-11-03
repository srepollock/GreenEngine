#include "RenderFileMessageReceiver.h"

RenderFileMessageReceiver::RenderFileMessageReceiver(std::vector<std::shared_ptr<Message>> *mq_p, std::mutex *mutex_p)
{
	_mq_p = mq_p;
	_mqMutex_p = mutex_p;
}

void RenderFileMessageReceiver::subscribeAll()
{
	this->subscribe(MESSAGE_TYPE::FileLoadedMessageType);
	this->subscribe(MESSAGE_TYPE::FileLoadedImageMessageType);
}

bool RenderFileMessageReceiver::messageHandler(std::shared_ptr<Message> message)
{
	_mqMutex_p->lock();
	_mq_p->push_back(message);
	_mqMutex_p->unlock();

	return false;
}
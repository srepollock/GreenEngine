#pragma once
#include <MessageReceiver.h>

class RenderFileMessageReceiver : MessageReceiver
{
public:
	RenderFileMessageReceiver(std::vector<std::shared_ptr<Message>> *mq_p, std::mutex *mutex);

	bool messageHandler(std::shared_ptr<Message> message) override;

	void subscribeAll();

private:
	std::mutex *_mqMutex_p;
	std::vector<std::shared_ptr<Message>> *_mq_p;
};
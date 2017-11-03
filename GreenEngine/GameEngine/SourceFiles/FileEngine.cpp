#include "FileEngine.h"
#include "Message.h"
#include "MessageTypes.h"
#include "FileHelper.h"
#include "MessagingSystem.h"

FileEngine::FileEngine()
{
	//we do all the hard work in start()
}

void FileEngine::start()
{
	//subscribe to messages
	subscribe(MESSAGE_TYPE::FileLoadMessageType);
	subscribe(MESSAGE_TYPE::FileLoadImageMessageType);

	//start loop
	_isRunning = true;
	_thread_p = new std::thread(&FileEngine::loop, this);
}

FileEngine::~FileEngine()
{
	_isRunning = false;
	_thread_p->join();
	delete(_thread_p);

	unsubscribe(MESSAGE_TYPE::FileLoadMessageType);
	unsubscribe(MESSAGE_TYPE::FileLoadImageMessageType);
}

void FileEngine::loop()
{
	//do setup on thread

	while (_isRunning)
	{
		for (int i = 0; i < CYCLE_MESSAGES_CONST; i++)
		{
			if (!_urgentMessageQueue.empty())
			{
				//handle urgent messages first
				_urgentMessageQueueMutex_p->lock();
				std::shared_ptr<Message> msg = _urgentMessageQueue.front();
				_urgentMessageQueue.pop();
				_urgentMessageQueueMutex_p->unlock();
				HandleMessage(msg);
				

			}
			else if (!_messageQueue.empty())
			{
				//then non-urgent messages
				_messageQueueMutex_p->lock();
				std::shared_ptr<Message> msg = _messageQueue.front();
				_messageQueue.pop();
				_messageQueueMutex_p->unlock();
				HandleMessage(msg);
				
			}
			else
			{
				//all empty, break
				break;
			}
		}		

		//avoid busywaiting!
		std::this_thread::sleep_for(std::chrono::milliseconds(CYCLE_DELAY_MS_CONST));
	}

	//do teardown on thread

}

void FileEngine::HandleMessage(std::shared_ptr<Message> inBaseMessage)
{	
	MESSAGE_TYPE contentType = inBaseMessage->getType();
	switch (contentType)
	{
		case MESSAGE_TYPE::FileLoadMessageType:
		{
			FileLoadMessageContent inMessage = *static_cast<FileLoadMessageContent*>(inBaseMessage->getContent());
			HandleNormalMessage(inMessage);
			break;
		}
		case MESSAGE_TYPE::FileLoadImageMessageType:
		{
			FileLoadImageMessageContent inMessage = *static_cast<FileLoadImageMessageContent*>(inBaseMessage->getContent());
			HandleImageMessage(inMessage);
			break;
		}
		default:
			SDL_Log("Filesystem: Received unknown message type");
			break;
	}
}

void FileEngine::HandleNormalMessage(FileLoadMessageContent inMessage)
{
	size_t hash = 0;
	std::string content = std::string();

	if (inMessage.relative)
	{
		hash = HashFilePath(inMessage.path, true);
		content = FileHelper::loadFileFromStringRelative(inMessage.path);
	}
	else
	{
		hash = HashFilePath(inMessage.path, false);
		content = FileHelper::loadFileFromString(inMessage.path);
	}

	FileLoadedMessageContent *outMessage = new FileLoadedMessageContent();

	outMessage->hash = hash;
	outMessage->content = content;
	outMessage->path = inMessage.path;
	outMessage->relative = inMessage.relative;

	std::shared_ptr<Message> message = std::make_shared<Message>(MESSAGE_TYPE::FileLoadedMessageType, false);
	message->setContent(outMessage);
	MessagingSystem::instance().postMessage(message);
}

void FileEngine::HandleImageMessage(FileLoadImageMessageContent inMessage)
{
	size_t hash = 0;
	SDL_Surface *content;

	if (inMessage.relative)
	{
		hash = HashFilePath(inMessage.path, true);
		content = FileHelper::loadImageFileFromStringRelative(inMessage.path);
	}
	else
	{
		hash = HashFilePath(inMessage.path, false);
		content = FileHelper::loadImageFileFromString(inMessage.path);
	}

	FileLoadedImageMessageContent *outMessage = new FileLoadedImageMessageContent();

	outMessage->hash = hash;
	outMessage->image = std::shared_ptr<SDL_Surface>(content, SDL_FreeSurface); //leave it this way
	outMessage->path = inMessage.path;
	outMessage->relative = inMessage.relative;

	std::shared_ptr<Message> message = std::make_shared<Message>(MESSAGE_TYPE::FileLoadedImageMessageType, false);
	message->setContent(outMessage);
	MessagingSystem::instance().postMessage(message);
}

size_t FileEngine::HashFilePath(std::string path, bool relative)
{
	std::string prefix;
	if (relative)
		prefix = "R|";
	else
		prefix = "A|";

	std::string strToHash = prefix + path;

	return std::hash<std::string>{}(strToHash);
}

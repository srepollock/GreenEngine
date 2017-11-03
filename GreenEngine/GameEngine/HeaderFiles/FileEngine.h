#pragma once
#include <cstdint>
#include <thread>
#include <atomic>
#include "MessageReceiver.h"

const int CYCLE_MESSAGES_CONST = 5;
const int_least64_t CYCLE_DELAY_MS_CONST = 10;

/// <summary>
/// Threaded file loading class that integrates with messaging system
/// </summary>
class FileEngine : MessageReceiver
{
public:	
	void start();
	FileEngine();
	~FileEngine();

	static size_t HashFilePath(std::string path, bool relative);
private:
	void loop();
	void HandleMessage(std::shared_ptr<Message> inBaseMessage);
	void HandleNormalMessage(FileLoadMessageContent inMessageContent);
	void HandleImageMessage(FileLoadImageMessageContent inMessageContent);

	std::atomic<bool> _isRunning;
	std::thread *_thread_p;
};
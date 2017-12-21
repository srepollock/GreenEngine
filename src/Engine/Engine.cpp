#include "Engine.h"
#include "ErrorHandler.h"
#include <typeinfo>

const int FRAMES_PER_SECOND = 120;


Engine::Engine() {
	
};

Engine::~Engine() {

};

std::thread* Engine::start() {
	subscribe(MESSAGE_TYPE::PhysicsReturnCall);
	subscribe(MESSAGE_TYPE::SceneDoneLoadType);
    // Create the other engines, or at least get pointer to them
	_fileSystem_p = new FileSystem();
	if (_fileSystem_p == nullptr) {
		std::cout << ErrorHandler::getErrorString(1) << std::endl;
	}
    _renderSystem_p = new RenderSystem();
    if (_renderSystem_p == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    _physicsSystem_p = new PhysicsSystem();
    if (_physicsSystem_p == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
	_inputSystem_p = new InputSystem();
	if (_inputSystem_p == nullptr) {
		std::cout << ErrorHandler::getErrorString(1) << std::endl;
	}
    /**_aiSystem_p = new AISystem();
    if (_aiSystem_p == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }**/
    _soundSystem_p = new SoundSystem();

    if (_soundSystem_p == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    try {
		_fileSystem_p->start();
        _renderSystem_p->start(); // Render handles it's own thread
        _physicsThread_p = _physicsSystem_p->start();
        //_aiThread_p = _aiSystem_p->start();
        //_soundSystem_p->start();		
    } catch (std::exception e) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }

    if (!_running) {
        _running = true;
    } else {
        this->stop();
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
        delete this;
    }

	_sceneObj = new Scene();
	while (checkMessages());
	_inputSystem_p->setUpInput();

	ticksAtLast = SDL_GetTicks();

	RenderLoadMessageContent *rlmc = new RenderLoadMessageContent();
	RenderableSetupData rsd;
	rsd.models.push_back("carModel");
	rsd.models.push_back("car2_body");
	rsd.models.push_back("car2_wheel");
	rsd.models.push_back("cone");
	rsd.models.push_back("track2a");
	rsd.models.push_back("track3b");
	rsd.models.push_back("cube");
	rsd.textures.push_back("car2_base");
	rsd.textures.push_back("car2_wheel");
	rsd.textures.push_back("grass");
	rsd.textures.push_back("grass2_d");
	rsd.textures.push_back("grass2_n");
	rsd.textures.push_back("asphalt");
	rsd.textures.push_back("test_texture3");
	rlmc->data = rsd;

	std::shared_ptr<Message> msg = std::make_shared<Message>(MESSAGE_TYPE::RenderLoadMessageType, false);
	msg->setContent(rlmc);
	MessagingSystem::instance().postMessage(msg);

	return new std::thread(&Engine::loop, this);
};
void Engine::update() {
	
	//run the renderer every tick

	uint32_t currentTime = SDL_GetTicks();
	if (currentTime > ticksAtLast + 1000 / FRAMES_PER_SECOND)
	{
		float delta = (((float_t)(currentTime - ticksAtLast)) / 1000);
		_inputSystem_p->checkInput(delta);
		if (_sceneObj != nullptr) {

			doWrites(delta);
			while (checkMessages()) {
				std::this_thread::yield();
			}
			doReads();
		}
		
		ticksAtLast = currentTime;
	}
	else {
		std::this_thread::yield();
	}
}

void Engine::loop() {

	if (!_running) 
	{
		return;
	}
	while (_running) 
	{
		this->update();
	}
	this->stop();
};
///
/// <title>
/// Engine Stop
/// </title>
/// <summary>
/// Will stop the engine from running. This will halt the main loop and other 
/// threads. The main purpose is to halt the game here. This can be a 
/// catch-all for other cleanup that is required in a stop/start of the game.
/// </summary>
/// 
void Engine::stop() 
{
	_inputSystem_p->~InputSystem();
	//_aiSystem_p->~AISystem();
	_physicsSystem_p->flagLoop();
	_physicsSystem_p->~PhysicsSystem();
    _renderSystem_p->~RenderSystem();
	
    _fileSystem_p->~FileSystem();

	_physicsThread_p->join();
	delete(_sceneObj);
    //_physicsSystem_p->stop();
    //_aiSystem_p->stop();
}

void Engine::flagLoop() 
{
	_running = false;
}

bool Engine::checkMessages()
{
	_urgentMessageQueueMutex_p->lock();
	_messageQueueMutex_p->lock();
	if (_messageQueue.empty() && _urgentMessageQueue.empty())
	{
		_urgentMessageQueueMutex_p->unlock();
		_messageQueueMutex_p->unlock();
	}
	else {
		if (!_urgentMessageQueue.empty())
		{
			_messageQueueMutex_p->unlock();
			// process an urgent message
			if (_urgentMessageQueue.front()->getType() == MESSAGE_TYPE::PhysicsReturnCall) {
				_urgentMessageQueueMutex_p->unlock();
				_urgentMessageQueue.pop();
				return false;
			}
			_urgentMessageQueueMutex_p->unlock();
		}
		else
		{
			_urgentMessageQueueMutex_p->unlock();
			if (!_messageQueue.empty())
			{
				// process a normal messages
				if (_messageQueue.front()->getType() == MESSAGE_TYPE::PhysicsReturnCall) {
					_messageQueueMutex_p->unlock();
					_messageQueue.pop();
					return false;
				}
				if (_messageQueue.front()->getType() == MESSAGE_TYPE::SceneDoneLoadType) {
					_messageQueueMutex_p->unlock();
					_messageQueue.pop();
					return false;
				}
			}
			_messageQueueMutex_p->unlock();
		}
	}
	
	return true;
}

void Engine::doWrites(GLfloat delta)
{
	PhysicsCallMessageContent *physicsContent = new PhysicsCallMessageContent(std::to_string(count++));
	physicsContent->worldObjects = _sceneObj->_worldObjects;
	physicsContent->deltaTime = delta;
	std::shared_ptr<Message> myMessage = std::make_shared<Message>(Message(MESSAGE_TYPE::PhysicsCallMessageType));
	myMessage->setContent(physicsContent);
	MessagingSystem::instance().postMessage(myMessage);
}

void Engine::doReads()
{
	RenderDrawMessageContent *renderContent = new RenderDrawMessageContent();
	renderContent->scene_p = _sceneObj->getRenderInformation();
	std::shared_ptr<Message> msg = std::make_shared<Message>(Message(MESSAGE_TYPE::RenderDrawMessageType, false));
	msg->setContent(renderContent);
	MessagingSystem::instance().postMessage(msg);
}

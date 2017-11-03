#include "Engine.h"
#include "ErrorHandler.h"
#include <typeinfo>

const int FRAMES_PER_SECOND = 60;

Engine::Engine() {
    
};

Engine::~Engine() {

};

std::thread* Engine::start() {
	
    // Create the other engines, or at least get pointer to them
	_fileEngine_p = new FileEngine();
	if (_fileEngine_p == nullptr) {
		std::cout << ErrorHandler::getErrorString(1) << std::endl;
	}
    _renderEngine_p = new RenderEngine();
    if (_renderEngine_p == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    _physicsEngine_p = new PhysicsEngine();
    if (_physicsEngine_p == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    /**_aiEngine_p = new AIEngine();
    if (_aiEngine_p == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }**/
    _inputEngine_p = new InputEngine();
    if (_inputEngine_p == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    _soundEngine_p = new SoundEngine();

    if (_soundEngine_p == nullptr) {
        std::cout << ErrorHandler::getErrorString(1) << std::endl;
    }
    try {
		_fileEngine_p->start();
        _renderEngine_p->start(); // Render handles it's own thread
        _physicsThread_p = _physicsEngine_p->start();
        //_aiThread_p = _aiEngine_p->start();
        _soundEngine_p->start();		
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

	ticksAtLast = SDL_GetTicks();

	RenderLoadMessageContent *rlmc = new RenderLoadMessageContent();
	RenderableSetupData rsd;
	rsd.models.push_back("cube");
	rsd.models.push_back("sphere");
	rsd.models.push_back("road_floor");
	rsd.models.push_back("carModel");
	rsd.textures.push_back("rainbow");
	rsd.textures.push_back("test_texture");
	rsd.textures.push_back("test_texture2");
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
		_inputEngine_p->checkInput();

		//SDL_Log("Ticked");
		if (_sceneObj != nullptr) {
			PhysicsCallMessageContent *physicsContent = new PhysicsCallMessageContent("Test");
			physicsContent->worldObjects = _sceneObj->_worldObjects;
			physicsContent->deltaTime = (((float_t)(currentTime - ticksAtLast)) / 1000);
			std::shared_ptr<Message> myMessage = std::make_shared<Message>(Message(MESSAGE_TYPE::PhysicsCallMessageType));
			myMessage->setContent(physicsContent);
			MessagingSystem::instance().postMessage(myMessage);

			RenderDrawMessageContent *renderContent = new RenderDrawMessageContent();
			renderContent->scene_p = _sceneObj->getRenderInformation();

			std::shared_ptr<Message> msg = std::make_shared<Message>(Message(MESSAGE_TYPE::RenderDrawMessageType, false));
			msg->setContent(renderContent);
			MessagingSystem::instance().postMessage(msg);
		}
		
		ticksAtLast = currentTime;
	}
	//SDL_Log("%s", "Running Engine::udpate");
}

void Engine::loop() {

	if (!_running) 
	{
		return;
	}
	while (_running) 
	{
		//SDL_Log("This one should work");

		this->update();

		//SDL_Log("Doing a stupid befpre!");

		//std::this_thread::sleep_for(std::chrono::milliseconds(2000));

		//SDL_Log("Doing a stupid!");

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
	//SDL_Log("Engine::stop");
	_soundEngine_p->~SoundEngine();
	_inputEngine_p->~InputEngine();
	//_aiEngine_p->~AIEngine();
	_physicsEngine_p->flagLoop();
	_physicsEngine_p->~PhysicsEngine();
    _renderEngine_p->~RenderEngine();
	
    _fileEngine_p->~FileEngine();

	_physicsThread_p->join();
	delete(_sceneObj);
    //_physicsEngine_p->stop();
    //_aiEngine_p->stop();
}

void Engine::flagLoop() 
{
	_running = false;
}
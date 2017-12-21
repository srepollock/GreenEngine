#pragma once
#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <SDL.h>
#include <SDL_mixer.h>
#endif
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include "FileSystem.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "AISystem.h"
#include "InputSystem.h"
#include "SoundSystem.h"
#include "ComponentHeaders.h"
#include "GameObject.h"
#include "Scene.h"
class Engine : public MessageReceiver {
public:
    std::thread* start();
    Engine();
    ~Engine();
    void update();
    void stop();
	void flagLoop();
	bool checkMessages();
	void doWrites(GLfloat);
	void doReads();
    std::thread *_engineThread_p;
private:
	FileSystem *_fileSystem_p;
    RenderSystem *_renderSystem_p;
    PhysicsSystem *_physicsSystem_p;
    AISystem *_aiSystem_p;
    InputSystem *_inputSystem_p;
    SoundSystem *_soundSystem_p;	
    std::thread *_physicsThread_p;
	//std::thread *_renderThread_p;
    //std::thread *_aiThread_p;
    bool _running = false;
	Scene* _sceneObj;
	float i = 0;
	uint32_t ticksAtLast = 0;
	int count = 0;
    void loop();
	
};

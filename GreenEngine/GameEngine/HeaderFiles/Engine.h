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
#include "EngineHeaders.h"
#include "ComponentHeaders.h"
#include "GameObject.h"
#include "Scene.h"
class Engine {
public:
    std::thread* start();
    Engine();
    ~Engine();
    void update();
    void stop();
	void flagLoop();
    std::thread *_engineThread_p;
private:
	FileEngine *_fileEngine_p;
    RenderEngine *_renderEngine_p;
    PhysicsEngine *_physicsEngine_p;
    AIEngine *_aiEngine_p;
    InputEngine *_inputEngine_p;
    SoundEngine *_soundEngine_p;	
    std::thread *_physicsThread_p;
	//std::thread *_renderThread_p;
    //std::thread *_aiThread_p;
    bool _running = false;
	Scene* _sceneObj;
	float i = 0;
	uint32_t ticksAtLast = 0;
    void loop();
	
};

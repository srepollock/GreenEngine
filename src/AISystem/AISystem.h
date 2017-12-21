#pragma once
#ifdef __APPLE__
#include <SDL2/SDL.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#endif
#include <thread>
class AISystem {
public:
    std::thread* start();
    void update();
    void stop();
    AISystem();
    ~AISystem();
private:
    bool _running = false;
    void loop();
};
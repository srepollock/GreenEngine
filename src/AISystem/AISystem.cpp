#include "AISystem.h"

std::thread* AISystem::start() {
    if (!_running) {
        _running = true;
    }
    return new std::thread(&AISystem::loop, this);
}

void AISystem::loop() {
    if (!_running) {
        return;
    }
    while(_running) {
        this->update();
    }
}

void AISystem::update() {
    //SDL_Log("%s", "Running AISystem::update");
}

void AISystem::stop() {
    _running = false;
}

AISystem::AISystem(void) {

}

AISystem::~AISystem(void) {

}
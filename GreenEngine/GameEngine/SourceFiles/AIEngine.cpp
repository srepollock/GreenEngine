#include "AIEngine.h"

std::thread* AIEngine::start() {
    if (!_running) {
        _running = true;
    }
    return new std::thread(&AIEngine::loop, this);
}

void AIEngine::loop() {
    if (!_running) {
        return;
    }
    while(_running) {
        this->update();
    }
}

void AIEngine::update() {
    //SDL_Log("%s", "Running AIEngine::update");
}

void AIEngine::stop() {
    _running = false;
}

AIEngine::AIEngine(void) {

}

AIEngine::~AIEngine(void) {

}
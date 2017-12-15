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
#include <memory>
#include "Message.h"
#include "MessagingSystem.h"

class SoundEngine : MessageReceiver {
public:
    SoundEngine();
    ~SoundEngine();
	void loop();

private:
	std::string basePath;
	std::map<std::string, Mix_Music*> gameSoundtracks;
	std::map<std::string, Mix_Chunk*> gameSoundEffects;
	void HandleMessage(std::shared_ptr<Message> inBaseMessage);
	void SoundMessage(SoundMessageContent inMessageContent);
};
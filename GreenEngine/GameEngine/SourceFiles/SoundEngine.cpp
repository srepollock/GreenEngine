#include "SoundEngine.h"

SoundEngine::SoundEngine()
{
	subscribe(MESSAGE_TYPE::SoundMessageType);
	SDL_Init(SDL_INIT_AUDIO);

	basePath = std::string(SDL_GetBasePath()) + "ResourceFiles/Sounds/";

	// Check frequency (22050 / 44100) and chunksize (2048 / 4096) values to adjust in case of sound lag.
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		SDL_Log("Error: %s", Mix_GetError(), "\n");
	}

	//TODO dynamic loading

	//Music
	gameSoundtracks.emplace("Music01", Mix_LoadMUS((basePath + "MenuMusic.mp3").c_str())); // Menu Screen
	gameSoundtracks.emplace("Music02", Mix_LoadMUS((basePath + "InGameMusic01.mp3").c_str())); // In Game 01
	gameSoundtracks.emplace("Music03", Mix_LoadMUS((basePath + "InGameMusic02.mp3").c_str())); // In Game 02
	gameSoundtracks.emplace("Music04", Mix_LoadMUS((basePath + "InGameMusic03.mp3").c_str())); // In Game 03
	gameSoundtracks.emplace("Music05", Mix_LoadMUS((basePath + "EndGameMusic.mp3").c_str())); // End Game Screen

	//SFX
	gameSoundEffects.emplace("SFX01", Mix_LoadWAV((basePath + "MenuMove.wav").c_str())); // Menu Move
	gameSoundEffects.emplace("SFX02", Mix_LoadWAV((basePath + "MenuSelect.wav").c_str())); // Menu Select
	gameSoundEffects.emplace("SFX03", Mix_LoadWAV((basePath + "VehicleMoving01.wav").c_str())); // Vehicle Moving 01
	gameSoundEffects.emplace("SFX04", Mix_LoadWAV((basePath + "VehicleMoving02.wav").c_str())); // Vehicle Moving 02 
	gameSoundEffects.emplace("SFX05", Mix_LoadWAV((basePath + "VehicleCollide.wav").c_str())); // Vehicle Colliding
	gameSoundEffects.emplace("SFX06", Mix_LoadWAV((basePath + "VehicleDestroyed.wav").c_str())); // Vehicle Destroyed
}

SoundEngine::~SoundEngine() 
{
	//Music
	Mix_FreeMusic(gameSoundtracks.at("Music01"));
	Mix_FreeMusic(gameSoundtracks.at("Music02"));
	Mix_FreeMusic(gameSoundtracks.at("Music03"));
	Mix_FreeMusic(gameSoundtracks.at("Music04"));
	Mix_FreeMusic(gameSoundtracks.at("Music05"));

	//SFX
	Mix_FreeChunk(gameSoundEffects.at("SFX01"));
	Mix_FreeChunk(gameSoundEffects.at("SFX02"));
	Mix_FreeChunk(gameSoundEffects.at("SFX03"));
	Mix_FreeChunk(gameSoundEffects.at("SFX04"));
	Mix_FreeChunk(gameSoundEffects.at("SFX05"));
	Mix_FreeChunk(gameSoundEffects.at("SFX06"));

	Mix_Quit();
	//SDL_Quit();
}

void SoundEngine::loop()
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
}

void SoundEngine::HandleMessage(std::shared_ptr<Message> inBaseMessage)
{
	MESSAGE_TYPE contentType = inBaseMessage->getType();

	if (contentType == MESSAGE_TYPE::SoundMessageType)
	{
		SoundMessageContent inMessage = *static_cast<SoundMessageContent*>(inBaseMessage->getContent());
		SoundMessage(inMessage);
	}
}

void SoundEngine::SoundMessage(SoundMessageContent inMessageContent)
{
	switch (inMessageContent.subType)
	{
		case S_TYPE::playMusic:
		{
			if (!Mix_PlayingMusic())
			{
				if(gameSoundtracks.count(inMessageContent.name) > 0)
					Mix_PlayMusic(gameSoundtracks.at(inMessageContent.name), -1);
			}
			else if (Mix_PausedMusic())
			{
				Mix_ResumeMusic();
			}
			else
			{
				Mix_PauseMusic();
			}
			break;
		}

		case S_TYPE::stopMusic:
		{
				Mix_HaltMusic();
				break;
		}

		case S_TYPE::playSound:
		{
			if (!Mix_Playing(-1))
			{
				//if (gameSoundEffects.count(inMessageContent.name) > 0)
					//Mix_PlayChannel(-1, gameSoundEffects.at(inMessageContent.name), 0);
					//Mix_PlayChannelTimed(-1, gameSoundEffects.at(inMessageContent.name), 0, 10);
				//break;
			}
		}
	}
}

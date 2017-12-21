#include "main.h" 
#include "Engine.h"
#include "GlobalPrefs.h"
#include "InputSystem.h"
#include "MessagingSystem.h"
#include "SoundSystem.h"

SDL_Window *g_window_p;
SDL_GLContext g_context;
std::thread* engineThread_p;

/// <summary>
/// Application entry point
/// </summary>
/// <param name="argc">Number of arguments passed to the application</param>
/// <param name="argv">Array containg string arguments passed to the application</param>
/// <return>Status code on application exit.</return>
int main(int argc, char ** argv) {
	SDL_Init(SDL_INIT_VIDEO);
	InputSystem *ie = new InputSystem(); // Start InputSystem
	SoundEngine *se = new SoundSystem();
	g_window_p = SDL_CreateWindow(
		GAME_NAME_CONST, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED, 
		GlobalPrefs::windowWidth, 
		GlobalPrefs::windowHeight, 
		SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI
	);
	MessagingSystem::instance().start();
	Engine *e = new Engine();
	engineThread_p = e->start();
	bool quit = false;
	SDL_Event ev;
	uint32_t ticksAtLast = SDL_GetTicks();
	while (!quit)
	{
		SDL_PollEvent(&ev);
		switch (ev.type)
		{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_CONTROLLERBUTTONUP:
				InputButtonDownContent *content = new InputButtonDownContent();
				content->ev = ev;
				std::shared_ptr<Message> myMessage = std::make_shared<Message>(Message(MESSAGE_TYPE::InputButtonDownCallType));
				myMessage->setContent(content);
				MessagingSystem::instance().postMessage(myMessage);
				break;
		}
		se->loop();
	}
	e->flagLoop();
	engineThread_p->join();
	delete(e);
	MessagingSystem::instance().kill();
	SDL_DestroyWindow(g_window_p);
	delete(se);
	return 0;
}
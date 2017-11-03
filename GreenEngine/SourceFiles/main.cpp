#include "main.h" 
#include "Engine.h"
#include "GlobalPrefs.h"
#include "InputEngine.h"
#include "MessagingSystem.h"

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
	//open opengl and window
	InputEngine *ie = new InputEngine();
	g_window_p = SDL_CreateWindow("RACE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GlobalPrefs::windowWidth, GlobalPrefs::windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

	// game code eventually goes here

	SDL_Log("It worked!");

	//it doesn't start Engine on a separate thread yet; Spencer I'll let you do that
	MessagingSystem::instance().start();
	Engine *e = new Engine();
	engineThread_p = e->start();

	//*****temporary loop stolen from racerender

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
				ie->buttonEventHandler(ev);
				break;
		}

		//run the renderer every tick
		/*uint32_t ticksSinceLast = SDL_GetTicks() - ticksAtLast;
		if (ticksSinceLast >= TICKS_TO_WAIT)
		{
			//e->update();
		}*/
	}

	

	//SDL_JoystickClose(joystick);

	SDL_Log("Main::Out of Loop");
	e->flagLoop();
	SDL_Log("Main::Wait for Engine Join");
	engineThread_p->join();
	SDL_Log("Engine::Thread Join");
	delete(e);
	MessagingSystem::instance().kill();
	SDL_DestroyWindow(g_window_p);

	//*****temporary loop section ends

	SDL_Quit();

	return 0;
}
#pragma once
#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2/SDL_gamecontroller.h>
#elif defined _WIN32 || defined _WIN64
#include <SDL.h>
#include <SDL_gamecontroller.h>
#endif

enum class INPUT_TYPES {
	LOOK_AXIS,
	MOVE_AXIS,
	TRIGGERS,
	Y_BUTTON,
	BACK_BUTTON
};
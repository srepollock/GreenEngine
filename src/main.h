#pragma once

#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <GLUT/glut.h>
#include <GLFW/glfw3.h>
#include <SDL2/SDL_gamecontroller.h>
#include "../GameEngine/HeaderFiles/FileHelper.h"
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_gamecontroller.h>
#include "..\GameEngine\HeaderFiles\FileHelper.h"
#endif 
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <limits>

extern SDL_Window *g_window_p;
extern SDL_GLContext g_context;
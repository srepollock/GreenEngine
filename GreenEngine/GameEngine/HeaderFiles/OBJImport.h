#pragma once

#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_filesystem.h>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <SDL.h>
#include <SDL_filesystem.h>
#endif 
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>

class OBJImport {
public:

	/// <summary>
	/// Loads info from an .obj file to vector GLfloat array to use for render engine
	///
	/// </summary>
	static std::vector<GLfloat> importObjInfo(std::string string);

};
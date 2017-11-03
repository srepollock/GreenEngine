#pragma once
#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <SDL.h>
#endif
#include "Vector3.h"
class Component
{
public:
    Component();
    Component(const Component &obj);
    //TODO: toString() method for each class? At least for name possibly
};
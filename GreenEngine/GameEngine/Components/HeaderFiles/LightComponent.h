#pragma once
#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <SDL.h>
#endif
#include "Component.h"
class LightComponent : public Component
{
public:
	GLfloat _intensity;
	// type // TODO: This needs to be different
	Vector3 _color;
	GLfloat _range;
	GLfloat _angle;
	LightComponent(GLfloat intense, Vector3 *color, GLfloat range, GLfloat angle) {
		_intensity = intense; _color = color; _range = range; _angle = angle;
	}
	LightComponent() {
		_intensity = 0.0f; _color = Vector3(0, 0, 0); _range = 0.0f; _angle = 0.0f;
	}
};
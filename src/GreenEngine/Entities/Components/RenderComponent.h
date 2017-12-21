#pragma once
#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <SDL.h>
#endif
#include <string>
#include "Component.h"
class RenderComponent : public Component
{
public:
	RenderComponent();
	RenderComponent(std::string _modelName, std::string _albedoName, std::string _normalName, GLfloat _smoothness);
	RenderComponent(const RenderComponent &obj);
	void setModelName(std::string _name);
	void setAlbedoName(std::string _name);
	void setNormalName(std::string _name);
	void setSmoothness(GLfloat _value);
	std::string getModelName();
	std::string getAlbedoName();
	std::string getNormalName();
	GLfloat getSmoothness();
private:
	std::string _modelName;
	std::string _albedoName;
	std::string _normalName;
	GLfloat _smoothness;
};
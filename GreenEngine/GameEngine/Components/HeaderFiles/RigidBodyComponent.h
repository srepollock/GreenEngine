#pragma once
#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <SDL.h>
#endif 
#include "Component.h"
class RigidBodyComponent : public Component
{
public:
	RigidBodyComponent();
	RigidBodyComponent(GLfloat _weight, GLfloat _bouncyness, GLfloat _friction, GLfloat _resistance);
	RigidBodyComponent(const RigidBodyComponent &obj);
	void setWeight(GLfloat _weight);
	void setBouncyness(GLfloat _bouncyness);
	void setFriction(GLfloat _friction);
	void setResistance(GLfloat _resistance);
	GLfloat getWeight();
	GLfloat getBouncyness();
	GLfloat getFriction();
	GLfloat getResistance();
private:
	GLfloat _weight;
	GLfloat _bouncyness;
	GLfloat _friction;
	GLfloat _resistance;
};
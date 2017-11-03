#pragma once
#include "Component.h"
class VelocityComponent : public Component
{
public:
	VelocityComponent();
	VelocityComponent(Vector3 _velocity, GLfloat _maxVelocity);
	VelocityComponent(Vector3 *_velocity, GLfloat _maxVelocity);
	VelocityComponent(const VelocityComponent &obj);
	void setVelocity(Vector3 _velocity);
	void setMaxVelocity(GLfloat _maxVelocity);
	Vector3 getVelocity();
	GLfloat getMaxVelocity();
	Vector3 _velocity;
	GLfloat _maxVelocity;
private:
};
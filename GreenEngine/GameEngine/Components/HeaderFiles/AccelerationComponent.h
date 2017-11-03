#pragma once
#include "Component.h"
class AccelerationComponent : public Component
{
public:
	AccelerationComponent();
	AccelerationComponent(Vector3 _acceleration, GLfloat _maxAcceleration);
	AccelerationComponent(Vector3 *_acceleration, GLfloat _maxAcceleration);
	AccelerationComponent(const AccelerationComponent &obj);
	void setAccelerationVector(Vector3 _acceleration);
	void setMaxAcceleration(GLfloat _maxAcceleration);
	Vector3 getAccelerationVector();
	GLfloat getMaxAcceleration();
	Vector3 _acceleration;
	GLfloat _maxAcceleration;
private:
};
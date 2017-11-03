#pragma once
#include "Component.h"
class CameraComponent : public Component
{
public:
	CameraComponent();
	CameraComponent(Vector3* _clearColor, GLfloat _nearPlane, GLfloat _farPlane, GLfloat _angle);
	CameraComponent(const CameraComponent &obj);
	void setClearColor(Vector3 _clearColor);
	void setNearPlane(GLfloat _nearPlane);
	void setFarPlane(GLfloat _farPlane);
	void setAngle(GLfloat _angle);
	Vector3 getClearColor();
	GLfloat getNearPlane();
	GLfloat getFarPlane();
	GLfloat getAngle();
private:
	Vector3 _clearColor;
	GLfloat _nearPlane;
	GLfloat _farPlane;
	GLfloat _angle;
};
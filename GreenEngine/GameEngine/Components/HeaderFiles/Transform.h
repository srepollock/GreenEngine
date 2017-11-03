#pragma once
#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <SDL.h>
#endif
#include "Vector3.h"
class Transform
{
public:
    Transform();
    Transform(Vector3 _position, Vector3 _rotation, GLfloat _scale);
    Transform(Vector3 *_position, Vector3 *_rotation, GLfloat _scale);
    Transform(const Transform &obj);
    void setPosition(Vector3 _position);
	void setRotation(Vector3 _rotation);
    void setScale(GLfloat _scale);
    Vector3 getPosition();
	Vector3 getRotation();
	GLfloat getScale();
	Vector3 getForward();
	void adjustDirections();
    Vector3 _position;
    Vector3 _rotation;
    GLfloat _scale;
	Vector3 _forward = Vector3(0, 0, 1);
	Vector3 _right = Vector3(1, 0, 0);
	Vector3 _up = Vector3(0, 1, 0);
private:
};
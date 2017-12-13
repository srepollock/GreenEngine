#pragma once
#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <SDL.h>
#endif
#include "Vector3.h"
#include "Quaternion.h"
class Transform
{
public:
    Transform();
    Transform(Vector3 _position, Vector3 _rotation, GLfloat _scale);
    Transform(Vector3 *_position, Vector3 *_rotation, GLfloat _scale);
    Transform(const Transform &obj);
	bool operator==(const Transform& t);
#pragma region Rotation Calculation Methods
	void rotateAround(Vector3 distance, Vector3 objectPos, Quaternion rotation);
	void rotate(Vector3 amount);
	void rotateQuat(Vector3 axis, GLfloat amount);
	void rotateX(GLfloat angle);
	void rotateY(GLfloat angle);
	void rotateZ(GLfloat angle);
#pragma endregion
#pragma region Translation Calculation Methods
	void translate(Vector3 translation);
	void translate(GLfloat x, GLfloat y, GLfloat z);
	void translateForward(GLfloat);
	void translateRight(GLfloat num);
#pragma endregion
    void setPosition(Vector3 _position);
	void setInitialPosition(Vector3 _parentPosition);
	void setRotation(Vector3 _rotation);
    void setScale(GLfloat _scale);
	void setDistanceToParent(Vector3 _distanceToParent);
    Vector3 getPosition();
	Vector3 getInitialPosition();
	Vector3 getRotation();
	GLfloat getScale();
	Vector3 getForward();
	Vector3 getDistanceToParent();
	void adjustDirections();
	////////////////////////////////
    Vector3 _position;
	Vector3 _initialPosition;
	Vector3 _distanceToParent;
    Vector3 _rotation;
    GLfloat _scale;
	Vector3 _forward;
	Vector3 _right;
	Vector3 _up;
	Quaternion _orientation;
private:
};
inline bool Transform::operator==(const Transform& t)
{
	return (this->_position == t._position &&
			this->_rotation == t._rotation &&
			this->_scale == t._scale);
};
#pragma once
#ifdef __APPLE__
#include <GL/glew.h>
#include <SDL2/SDL.h>
#elif defined _WIN32 || defined _WIN64
#include <glew.h>
#include <SDL.h>
#endif 
#include "Component.h"
#include "Quaternion.h"
#define GRAVITY -9.81
class RigidBodyComponent : public Component
{
public:
	RigidBodyComponent();
	RigidBodyComponent(GLfloat _maxAccel, GLfloat _maxVel, GLfloat _mass, GLfloat _bouncyness, GLfloat _friction, GLfloat _resistance, Vector3 dimensions);
	RigidBodyComponent(const RigidBodyComponent &obj);
	void setMass(GLfloat _mass);
	void setBouncyness(GLfloat _bouncyness);
	void setFriction(GLfloat _friction);
	void setResistance(GLfloat _resistance);
	void setVelocity(Vector3 _velocity);
	void setMaxVelocity(GLfloat _maxVelocity);
	void setAccelerationVector(Vector3 _acceleration);
	void setTurningDegree(GLfloat _num);
	void setForce(Vector3 _num);
	void setAngularAccel(Vector3 _vec);
	GLfloat getMass();
	GLfloat getBouncyness();
	GLfloat getFriction();
	GLfloat getResistance();
	Vector3 getAccelerationVector();
	Vector3 getVelocity();
	GLfloat getMaxVelocity();
	GLfloat getTurningDegree();
	Vector3 getForce();
	Vector3 getAngularAccel();
	void calculatemInteria();
	GLfloat _width, _height, _length;
	Vector3 _angularVel;
	Vector3 _angularMoment;
	glm::mat3x3 _mInertia, _mInertiaInverse;
private:
	GLfloat _mass;
	GLfloat _bouncyness;
	GLfloat _friction;
	GLfloat _resistance;
	Vector3 _acceleration;
	Vector3 _velocity;
	GLfloat _maxVelocity;
	GLfloat _turningDegree;
	Vector3 _force;

	Vector3 _angularAccel;
};

inline void RigidBodyComponent::calculatemInteria() 
{
	GLfloat Ixx, Iyy, Izz;

	Ixx = (_mass / 12.0f) * (_width * _width + _height * _height);
	Iyy = (_mass / 12.0f) * (_height * _height + _length * _length);
	Izz = (_mass / 12.0f) * (_width * _width + _length * _length);

	_mInertia = glm::mat3x3(glm::vec3(Ixx, 0.0f, 0.0f), glm::vec3(0.0f, Iyy, 0.0f), glm::vec3(0.0f, 0.0f, Izz));
	_mInertiaInverse = glm::inverse(_mInertia);
};
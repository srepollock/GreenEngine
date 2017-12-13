#include "RigidBodyComponent.h"
RigidBodyComponent::RigidBodyComponent()
{
    this->_mass = 50;
    this->_bouncyness = 0;
    this->_friction = 0;
    this->_resistance = 0;
	this->_maxVelocity = 10;
	this->_turningDegree = 0;
	this->_velocity = new Vector3();
	this->_acceleration = new Vector3();
	this->_height = this->_length = this->_width = 1.0f;
	this->_angularVel = Vector3();
	this->_angularMoment = Vector3();
	this->_force = Vector3();
	this->_angularAccel = Vector3();
	calculatemInteria();
};
RigidBodyComponent::RigidBodyComponent(GLfloat _maxAccel, GLfloat _maxVel, GLfloat _weight, GLfloat _bouncyness, GLfloat _friction, GLfloat _resistance, Vector3 dimensions)
{
    this->_mass = _weight / -GRAVITY;
    this->_bouncyness = _bouncyness;
    this->_friction = _friction;
    this->_resistance = _resistance;
	this->_maxVelocity = _maxVel;
	this->_turningDegree = 0;
	this->_width = dimensions.x;
	this->_height = dimensions.y;
	this->_length = dimensions.z;
	this->_velocity = new Vector3();
	this->_acceleration = new Vector3();
	this->_angularVel = Vector3();
	this->_angularMoment = Vector3();
	this->_force = Vector3();
	this->_angularAccel = Vector3();
	calculatemInteria();
};
RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent &obj)
{
    this->_mass = obj._mass;
    this->_bouncyness = obj._bouncyness;
    this->_friction = obj._friction;
    this->_resistance = obj._resistance;
	this->_maxVelocity = obj._maxVelocity;
	this->_turningDegree = obj._turningDegree;
	this->_velocity = obj._velocity;
	this->_acceleration = obj._acceleration;
	this->_height = obj._height; this->_length = obj._length;
	this->_width = obj._width;
	this->_angularVel = obj._angularVel;
	this->_angularMoment = obj._angularMoment;
	this->_force = obj._force;
	this->_angularAccel = obj._angularAccel;
	calculatemInteria();
};
void RigidBodyComponent::setMass(GLfloat _mass)
{
    this->_mass = _mass;
};
void RigidBodyComponent::setBouncyness(GLfloat _bouncyness)
{
    this->_bouncyness = _bouncyness;
};
void RigidBodyComponent::setFriction(GLfloat _friction)
{
    this->_friction = _friction;
};
void RigidBodyComponent::setResistance(GLfloat _resistance)
{
    this->_resistance = _resistance;
};
GLfloat RigidBodyComponent::getMass()
{
    return this->_mass;
};
GLfloat RigidBodyComponent::getBouncyness()
{
    return this->_bouncyness;
};
GLfloat RigidBodyComponent::getFriction()
{
    return this->_friction;
};
GLfloat RigidBodyComponent::getResistance()
{
    return this->_resistance;
};
void RigidBodyComponent::setAccelerationVector(Vector3 _acceleration)
{
	this->_acceleration = _acceleration;
};
void RigidBodyComponent::setTurningDegree(GLfloat _num) 
{
	this->_turningDegree = _num;
};
void RigidBodyComponent::setForce(Vector3 _num) 
{
	this->_force = _num;
};
void RigidBodyComponent::setAngularAccel(Vector3 _vec)
{
	this->_angularAccel = _vec;
};
Vector3 RigidBodyComponent::getAccelerationVector()
{
	return this->_acceleration;
};
void RigidBodyComponent::setVelocity(Vector3 _velocity)
{
	this->_velocity = _velocity;
};
void RigidBodyComponent::setMaxVelocity(GLfloat _maxVelocity)
{
	this->_maxVelocity = _maxVelocity;
};
Vector3 RigidBodyComponent::getVelocity()
{
	return this->_velocity;
};
GLfloat RigidBodyComponent::getMaxVelocity()
{
	return this->_maxVelocity;
};
GLfloat RigidBodyComponent::getTurningDegree() 
{
	return this->_turningDegree;
};
Vector3 RigidBodyComponent::getForce() 
{
	return this->_force;
};
Vector3 RigidBodyComponent::getAngularAccel()
{
	return _angularAccel;
};

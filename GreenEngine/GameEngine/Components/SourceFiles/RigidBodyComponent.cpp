#include "RigidBodyComponent.h"
RigidBodyComponent::RigidBodyComponent()
{
    this->_weight = 0;
    this->_bouncyness = 0;
    this->_friction = 0;
    this->_resistance = 0;
};
RigidBodyComponent::RigidBodyComponent(GLfloat _weight, GLfloat _bouncyness, GLfloat _friction, GLfloat _resistance)
{
    this->_weight = _weight;
    this->_bouncyness = _bouncyness;
    this->_friction = _friction;
    this->_resistance = _resistance;
};
RigidBodyComponent::RigidBodyComponent(const RigidBodyComponent &obj)
{
    this->_weight = obj._weight;
    this->_bouncyness = obj._bouncyness;
    this->_friction = obj._friction;
    this->_resistance = obj._resistance;
};
void RigidBodyComponent::setWeight(GLfloat _weight)
{
    this->_weight = _weight;
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
GLfloat RigidBodyComponent::getWeight()
{
    return this->_weight;
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
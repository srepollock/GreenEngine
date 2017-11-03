#include "GameObject.h"
GameObject::GameObject()
{
    this->_transform = Transform();
    this->_components_p = new std::map<std::string, Component *>();
};
GameObject::GameObject(Transform transform)
{
	this->_transform = transform;
	this->_components_p = new std::map<std::string, Component *>();
};
GameObject::GameObject(Transform *transform)
{
	this->_transform = *transform;
	this->_components_p = new std::map<std::string, Component *>();
};
std::map<std::string, Component *> GameObject::getComponentList()
{
    return *this->_components_p;
};
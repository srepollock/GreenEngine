#include "GameObject.h"
GameObject::GameObject()
{
    this->_transform = Transform();
	this->_name = "";
    this->_components_p = new std::map<std::string, Component *>();
	this->_childObjects_p = new std::vector<GameObject *>();
};
GameObject::GameObject(std::string name)
{
    this->_transform = Transform();
	this->_name = name;
    this->_components_p = new std::map<std::string, Component *>();
	this->_childObjects_p = new std::vector<GameObject *>();
};
GameObject::GameObject(Transform transform)
{
	this->_transform = transform;
	this->_name = "";
	this->_components_p = new std::map<std::string, Component *>();
	this->_childObjects_p = new std::vector<GameObject *>();
};
GameObject::GameObject(Transform *transform)
{
	this->_transform = *transform;
	this->_name = "";
	this->_components_p = new std::map<std::string, Component *>();
	this->_childObjects_p = new std::vector<GameObject *>();
};
GameObject::GameObject(Transform transform, std::string name)
{
	this->_transform = transform;
	this->_name = name;
	this->_components_p = new std::map<std::string, Component *>();
	this->_childObjects_p = new std::vector<GameObject *>();
};
GameObject::GameObject(Transform *transform, std::string name)
{
	this->_transform = *transform;
	this->_name = name;
	this->_components_p = new std::map<std::string, Component *>();
	this->_childObjects_p = new std::vector<GameObject *>();
};
void GameObject::setName(std::string name)
{
	this->_name = name;
};
std::string GameObject::getName()
{
	return this->_name;
};
std::map<std::string, Component *> GameObject::getComponentList()
{
    return *this->_components_p;
};
void GameObject::addChild(GameObject *child)
{
	child->_transform._distanceToParent = child->_transform._position - this->_transform._position;
	this->_childObjects_p->push_back(child);
};
GameObject* GameObject::getChild(GameObject *child)
{
	for (auto &go : *(this->_childObjects_p))
	{
		if (*go == *child)
		{
			return go;
		}
	}
	return nullptr;
};
GameObject* GameObject::getChild(std::string name)
{
	for (auto &go : *(this->_childObjects_p))
	{
		if (go->_name == name)
		{
			return go;
		}
	}
	return nullptr;
};
void GameObject::removeChild(GameObject *child)
{
	for (std::vector<GameObject *>::iterator i = this->_childObjects_p->begin();
			i != this->_childObjects_p->end();)
	{
		if (**i == *child)
		{
			i = this->_childObjects_p->erase(i);
		}
		else
		{
			++i;
		}
	}
};
std::vector<GameObject *> GameObject::getChildObjectList()
{
	return *this->_childObjects_p;
};
void GameObject::translate(Vector3 vec)
{
	this->_transform.translate(vec);
	for (std::vector<GameObject *>::iterator i = this->_childObjects_p->begin();
		i != this->_childObjects_p->end();)
	{
		if ((*i)->_name == "up" || (*i)->_name == "forward" || (*i)->_name == "right") 
		{
			this->updateDirectionPositions(i);
		}
		i++;
	}
};
void GameObject::rotate(Vector3 vec)
{
	this->_transform.rotate(vec);
	for (std::vector<GameObject *>::iterator i = this->_childObjects_p->begin();
		i != this->_childObjects_p->end();)
	{
		(*i)->_transform._orientation = this->_transform._orientation * MakeQFromEulerAngles((*i)->_transform._rotation);
		if ((*i)->_name != "up" || (*i)->_name != "forward" || (*i)->_name != "right") 
		{
			this->rotateAroundParent(i);
		}
		i++;
	}
};

void GameObject::rotate(Vector3 vec, GLfloat angle)
{
	this->_transform.rotateQuat(vec, angle);
	for (std::vector<GameObject *>::iterator i = this->_childObjects_p->begin();
		i != this->_childObjects_p->end();)
	{
		(*i)->_transform._orientation = this->_transform._orientation * MakeQFromEulerAngles((*i)->_transform._rotation);
		if ((*i)->_name != "up" || (*i)->_name != "forward" || (*i)->_name != "right")
		{
			this->rotateAroundParent(i);
		}
		i++;
	}
};

void GameObject::updateDirectionPositions(std::vector<GameObject *>::iterator i)
{
	if ((*i)->_name == "up")
		(*i)->_transform._position = this->_transform._position + this->_transform._up;
	else if ((*i)->_name == "forward")
		(*i)->_transform._position = this->_transform._position + this->_transform._forward;
	else if ((*i)->_name == "right")
		(*i)->_transform._position = this->_transform._position + this->_transform._right;
};

void GameObject::rotateAroundParent(std::vector<GameObject*>::iterator i) 
{
	(*i)->_transform._position = QVRotate(this->_transform._orientation, (*i)->_transform._distanceToParent) + this->_transform._position;
};

void GameObject::updateChildPositions(std::vector<GameObject*>::iterator i) 
{
	//(*i)->_transform._position = (*i)->_transform._distanceToParent + this->_transform._position;
};
#pragma once
#include <list>
//#include <vector>
#include <map>
#include <typeinfo>
#include <typeindex>
#include "Transform.h"
#include "ComponentHeaders.h"
class GameObject
{
public:
	GameObject();
	GameObject(Transform transform);
	GameObject(Transform *transform);
	Transform _transform;
	template <typename T>
	void addComponent(T component)
	{
		std::string type = this->getType<T>();
		(*this->_components_p)[type] = component;
	};
	template <typename T>
	void removeComponent()
	{
		std::string type = getType<T>();
		if (this->hasComponent<T>())
		{
			this->_components_p->erase(this->_components_p->find(type));
		}
	};
	template <typename T>
	T getComponent()
	{
		if (this->hasComponent<T>())
		{
			return (T)(*this->_components_p)[this->getType<T>()]; // TODO: Dynamic cast?
		}
		else
		{
			return NULL;
		}
	};
	template <typename T>
	bool hasComponent()
	{
		if (this->_components_p->find(this->getType<T>()) == this->_components_p->end())
		{
			return false;
		}
		else
		{
			return true;
		}
	};
	std::map<std::string, Component *> getComponentList();
private:
	std::map<std::string, Component *> *_components_p;
	template <typename T>
	std::string getType()
	{
		return typeid(T).name();
	};
};
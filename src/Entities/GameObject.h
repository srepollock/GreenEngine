#pragma once
#include <list>
#include <vector>
#include <map>
#include <typeinfo>
#include <typeindex>
#include <mutex>
#include "Transform.h"
#include "ComponentHeaders.h"
class GameObject
{
public:
	GameObject(); // TODO: Add name as "identifier" for GameObject
	GameObject(std::string name);
	GameObject(Transform transform);
	GameObject(Transform *transform);
	GameObject(Transform transform, std::string name);
	GameObject(Transform *transform, std::string name);
	std::string _name;
	Transform _transform;
	void setName(std::string name);
	std::string getName();
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
	template <typename T>
	bool compareMaps(const std::map<std::string,T>& l, const std::map<std::string,T>& r)
	{
	  // same types, proceed to compare maps here
	  if(l.size() != r.size())
	  {
		return false;  // differing sizes, they are not the same
	  }
	  typename std::map<std::string,T>::const_iterator i, j;
	  for(i = l.begin(), j = r.begin(); i != l.end(); ++i, ++j)
	  {
		if(*i != *j)
		{
			return false;
		}
	  }
	  return true;
	};
	std::map<std::string, Component *> getComponentList();
	void addChild(GameObject *child);
	GameObject* getChild(GameObject *child);
	GameObject* getChild(std::string name);
	void removeChild(GameObject *child);
	std::vector<GameObject *> getChildObjectList();
	void translate(Vector3 vec);
	void rotate(Vector3 vec);
	// TODO: Rotate all child objects about this position
	void rotate(Vector3 vec, GLfloat angle);
	void updateDirectionPositions(std::vector<GameObject*>::iterator i);
	void rotateAroundParent(std::vector<GameObject*>::iterator i);
	void updateChildPositions(std::vector<GameObject*>::iterator i);
	bool operator==(GameObject& go);
private:
	std::map<std::string, Component *> *_components_p;
	std::vector<GameObject *> *_childObjects_p;
	template <typename T>
	std::string getType()
	{
		return typeid(T).name();
	};
};
inline bool GameObject::operator==(GameObject& go)
{
	if (!(this->_transform == go._transform))
	{
		return false;
	}
	if (!(this->_name == go._name))
	{
		return false;
	}
	// Check each component // REVIEW: How do I write this??
	/**
	if (this->getComponentList() != NULL && go.getComponentList() != NULL)
	{
		bool compEqual;
		for (Component *comp : this->getComponentList())
		{
			compEqaul = false;
			for (Component *comp2 : this->getComponentList())
			{
				if (*comp == *comp2)
				{
					compEqual = true;
				}
			}
			if (!compEqual)
			{
				return false;
			}
		}
		if (!compEqual)
		{
			return false;
		}
	}
	*/
	// Check each child
	if (!(this->getChildObjectList().empty()) && !(go.getChildObjectList().empty()))
	{
		bool childEqual;
		for (GameObject *child : this->getChildObjectList())
		{
			childEqual = false;
			for (GameObject *child2 : go.getChildObjectList())
			{
				if (*child == *child2)
				{
					childEqual = true;
				}
			}
			if (!childEqual)
			{
				return false; // No similar children found
			}
		}
		if (!childEqual)
		{
			return false;
		}
	}
	return true;
};
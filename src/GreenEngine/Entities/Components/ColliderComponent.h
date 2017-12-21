#pragma once
#include "Component.h"
class ColliderComponent : public Component
{
public:
	ColliderComponent();
	ColliderComponent(Vector3 _vertices);
	ColliderComponent(Vector3 *_vertices);
	ColliderComponent(const ColliderComponent& obj);
	bool operator==(const ColliderComponent& cc) const;
	void setVertices(Vector3 _vertices);
	Vector3 getVertices();
private:
	Vector3 _vertices; // TODO: This needs to be different
};
inline bool ColliderComponent::operator==(const ColliderComponent& cc) const
{
	return this->_vertices == cc._vertices;
};
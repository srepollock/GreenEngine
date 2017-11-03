#pragma once
#include "Component.h"
class ColliderComponent : public Component
{
public:
	ColliderComponent();
	ColliderComponent(Vector3 _vertices);
	ColliderComponent(Vector3 *_vertices);
	ColliderComponent(const ColliderComponent& obj);
	void setVertices(Vector3 _vertices);
	Vector3 getVertices();
private:
	Vector3 _vertices; // TODO: This needs to be different
};
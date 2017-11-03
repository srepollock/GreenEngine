#include "ColliderComponent.h"
ColliderComponent::ColliderComponent()
{
    this->_vertices = new Vector3(); // TODO: Fix this
};
ColliderComponent::ColliderComponent(Vector3 _vertices)
{
    this->_vertices = _vertices;
};
ColliderComponent::ColliderComponent(Vector3 *_vertices)
{
    this->_vertices = *_vertices;
};
ColliderComponent::ColliderComponent(const ColliderComponent& obj)
{
    this->_vertices = obj._vertices;
};
void ColliderComponent::setVertices(Vector3 _vertices)
{
    this->_vertices = _vertices;
};
Vector3 ColliderComponent::getVertices()
{
    return this->_vertices;
};
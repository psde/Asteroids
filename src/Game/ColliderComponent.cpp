#include "ColliderComponent.h"

#include <cmath>
#include <iostream>

namespace Game
{
	ColliderComponent::ColliderComponent()
	{
	}

	void ColliderComponent::setCollisionMesh(std::unique_ptr<Geometry::Mesh> mesh)
	{
		_mesh = std::move(mesh);
	}

	bool ColliderComponent::collidesWith(const ColliderComponent *other) const
	{
		glm::vec4 a = _mesh->getBoundingBox() + glm::vec4(_position, _position);
		glm::vec4 b = other->_mesh->getBoundingBox() + glm::vec4(other->_position, other->_position);

		if (a.z < b.x || a.x > b.z) return false;
		if (a.w < b.y || a.y > b.w) return false;

		return true;
	}
}
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

	bool ColliderComponent::collidesWithAABB(const ColliderComponent *other) const
	{
		glm::vec4 a = _mesh->getBoundingBox() + glm::vec4(_position, _position);
		glm::vec4 b = other->_mesh->getBoundingBox() + glm::vec4(other->_position, other->_position);

		if (a.z < b.x || a.x > b.z) return false;
		if (a.w < b.y || a.y > b.w) return false;

		return true;
	}

	bool ColliderComponent::isIntersecting(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d) const
	{
		float denominator = ((b.x - a.x) * (d.y - c.y)) - ((b.y - a.y) * (d.x - c.x));
		float numerator1 = ((a.y - c.y) * (d.x - c.x)) - ((a.x - c.x) * (d.y - c.y));
		float numerator2 = ((a.y - c.y) * (b.x - a.x)) - ((a.x - c.x) * (b.y - a.y));

		if (denominator == 0) return numerator1 == 0 && numerator2 == 0;

		float r = numerator1 / denominator;
		float s = numerator2 / denominator;

		return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
	}

	bool ColliderComponent::collidesWith(const ColliderComponent *other) const
	{
		if (collidesWithAABB(other) == false)
			return false;

		int numVertsA = _mesh->vertices().size();
		int numVertsB = other->_mesh->vertices().size();

		for (int a1 = 0; a1 < numVertsA; a1++) {
			int a2 = (a1 + 1) % numVertsA;
			for (int b1 = 0; b1 < numVertsB; b1++) {
				int b2 = (b1 + 1) % numVertsB;
				if (isIntersecting(_mesh->vertices().at(a1) + _position, _mesh->vertices().at(a2) + _position,
					other->_mesh->vertices().at(b1) + other->_position, other->_mesh->vertices().at(b2) + other->_position))
					return true;
			}
		}
		return false;
	}
}
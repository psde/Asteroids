#include "CollisionComponent.h"

#include <cmath>
#include <iostream>

namespace Components
{
	CollisionComponent::CollisionComponent()
	{
	}

	void CollisionComponent::setCollisionMesh(Graphics::Mesh* mesh)
	{
		m_mesh = mesh;
	}

	bool CollisionComponent::collidesWithAABB(const CollisionComponent *other) const
	{
		for (int y = -1; y <= 1; ++y)
		{
			for (int x = -1; x <= 1; ++x)
			{
				Math::vec4 a = m_mesh->boundingBox() + Math::vec4(m_position + Math::vec2(800 * x, 600 * y), m_position + Math::vec2(800 * x, 600 * y));
				Math::vec4 b = other->m_mesh->boundingBox() + Math::vec4(other->m_position, other->m_position);

				if (a.z >= b.x && a.x <= b.z && a.w >= b.y && a.y <= b.w) return true;
			}
		}

		return false;
	}

	bool CollisionComponent::isIntersecting(Math::vec2 a, Math::vec2 b, Math::vec2 c, Math::vec2 d) const
	{
		float denominator = ((b.x - a.x) * (d.y - c.y)) - ((b.y - a.y) * (d.x - c.x));
		float numerator1 = ((a.y - c.y) * (d.x - c.x)) - ((a.x - c.x) * (d.y - c.y));
		float numerator2 = ((a.y - c.y) * (b.x - a.x)) - ((a.x - c.x) * (b.y - a.y));

		if (denominator == 0) return numerator1 == 0 && numerator2 == 0;

		float r = numerator1 / denominator;
		float s = numerator2 / denominator;

		return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
	}

	bool CollisionComponent::collidesWith(const CollisionComponent *other) const
	{
		if (!collidesWithAABB(other))
			return false;
		
		auto myVertices = m_mesh->vertices();
		auto otherVertices = other->m_mesh->vertices();

		for (auto a1 = std::begin(myVertices); a1 != std::end(myVertices); a1++) {
			auto a2 = a1 + 1;
			if (a2 == std::end(myVertices))
				a2 = std::begin(myVertices);

			for (auto b1 = std::begin(otherVertices); b1 != std::end(otherVertices); b1++) {
				auto b2 = b1 + 1;
				if (b2 == std::end(otherVertices))
					b2 = std::begin(otherVertices);

				for (int y = -1; y <= 1; ++y)
				{
					for (int x = -1; x <= 1; ++x)
					{
						if (isIntersecting(*a1 + m_position + Math::vec2(800 * x, 600 * y), *a2 + m_position + Math::vec2(800 * x, 600 * y), *b1 + other->m_position, *b2 + other->m_position))
						{
							return true;
						}
					}
				}
			}
		}
		return false;
	}
}
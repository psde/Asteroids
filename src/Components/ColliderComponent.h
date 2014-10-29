#pragma once

#include <glm/glm.hpp>

#include "Geometry/Mesh.h"

namespace Components
{
	class ColliderComponent
	{
	private:
		Geometry::Mesh* _mesh;
		glm::vec2 _position;

		bool collidesWithAABB(const ColliderComponent *other) const;

		bool isIntersecting(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d) const;

	public:
		ColliderComponent();

		void setCollisionMesh(Geometry::Mesh* mesh);
		void setPosition(glm::vec2 pos) { _position = pos; }

		bool collidesWith(const ColliderComponent *other) const;
	};
}
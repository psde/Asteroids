#pragma once

#include <glm/glm.hpp>

#include "Geometry/Mesh.h"

namespace Game
{
	class ColliderComponent
	{
	private:
		std::unique_ptr<Geometry::Mesh> _mesh;
		glm::vec2 _position;

	public:
		ColliderComponent();

		void setCollisionMesh(std::unique_ptr<Geometry::Mesh> mesh);
		void setPosition(glm::vec2 pos) { _position = pos; }

		bool collidesWith(const ColliderComponent *other) const;
	};
}
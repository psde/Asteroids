#pragma once

//#include <glm/glm.hpp>
#include "Math/Math.h"

#include "Geometry/Mesh.h"

namespace Components
{
	// Provides line to line collision detection, by first performing an
	// AABB collision test, and then checking each line from the mesh to the
	// each line of the other mesh.
	class ColliderComponent
	{
	private:
		Geometry::Mesh* _mesh;
		glm::vec2 _position;

		bool collidesWithAABB(const ColliderComponent *other) const;

		bool isIntersecting(glm::vec2 a, glm::vec2 b, glm::vec2 c, glm::vec2 d) const;

	public:
		ColliderComponent();

		// Sets the collision mesh to the given mesh
		void setCollisionMesh(Geometry::Mesh* mesh);

		// Sets the position of the collision mesh
		void setPosition(glm::vec2 pos) { _position = pos; }

		// Returns true if the other ColliderComponent collides with this component
		bool collidesWith(const ColliderComponent *other) const;
	};
}
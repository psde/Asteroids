#pragma once

//#include <Math/Math.hpp>
#include "Math/Math.h"

#include "Graphics/Geometry/Mesh.h"

namespace Components
{
	// Provides line to line collision detection, by first performing an
	// AABB collision test, and then checking each line from the mesh to the
	// each line of the other mesh.
	class ColliderComponent
	{
	private:
		Geometry::Mesh* _mesh;
		Math::vec2 _position;

		bool collidesWithAABB(const ColliderComponent *other) const;

		bool isIntersecting(Math::vec2 a, Math::vec2 b, Math::vec2 c, Math::vec2 d) const;

	public:
		ColliderComponent();

		// Sets the collision mesh to the given mesh
		void setCollisionMesh(Geometry::Mesh* mesh);

		// Sets the position of the collision mesh
		void setPosition(Math::vec2 pos) { _position = pos; }

		// Returns true if the other ColliderComponent collides with this component
		bool collidesWith(const ColliderComponent *other) const;
	};
}
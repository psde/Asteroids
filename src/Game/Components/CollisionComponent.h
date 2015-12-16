#pragma once

//#include <Math/Math.hpp>
#include "Math/Math.h"

#include "Graphics/Geometry/Mesh.h"

namespace Components
{
	// Provides line to line collision detection, by first performing an
	// AABB collision test, and then checking each line from the mesh to the
	// each line of the other mesh.
	class CollisionComponent
	{
	private:
		Graphics::Mesh* m_mesh;
		Math::vec2 m_position;

		bool collidesWithAABB(const CollisionComponent *other) const;

		bool isIntersecting(Math::vec2 a, Math::vec2 b, Math::vec2 c, Math::vec2 d) const;

	public:
		CollisionComponent();
		virtual ~CollisionComponent() = default;

		// Sets the collision mesh to the given mesh
		void setCollisionMesh(Graphics::Mesh* mesh);

		// Sets the position of the collision mesh
		void setPosition(Math::vec2 pos) { m_position = pos; }

		// Returns true if the other CollisionComponent collides with this component
		bool collidesWith(const CollisionComponent *other) const;
	};
}

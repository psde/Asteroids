#pragma once

#include <memory>

#include "Math/Math.h"

#include "Graphics/Geometry/Mesh.h"

#include "GameObject.h"
#include "Components/PhysicsEuler.h"
#include "Components/CollisionComponent.h"

namespace Game
{
	// Implements the projectiles which can be launched from the ship
	class Projectile : public GameObject
	{
	private:
		std::unique_ptr<Graphics::Mesh> m_mesh;
		std::shared_ptr<Graphics::Program> m_shader;

		bool m_launched;
		float m_time;
		float m_lifetime;
		bool m_friendly;

	public:
		Projectile(float lifetime, bool friendly);

		// Advances the projectile simulation by the given time step
		void update(float delta) override;

		// Draws the projectile at the current position
		void draw() override;

		// Returns true if the projectile is currently in flight 
		bool launched();

		// Launches the projectile from the given position in the given direction
		void shoot(Math::vec2 position, Math::vec2 direction);

		// Reloads the projectile, allowing it to be fired again
		void reload();

		bool friendly() { return m_friendly; }
	};
}

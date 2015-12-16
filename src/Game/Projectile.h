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
		std::unique_ptr<Graphics::Mesh> _mesh;
		std::shared_ptr<Graphics::Program> _shader;

		bool _launched;
		float _time;
		float _lifetime;
		bool _friendly;

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

		bool friendly() { return _friendly; }
	};
}
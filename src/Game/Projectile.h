#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Geometry/Mesh.h"

#include "GameObject.h"
#include "Components/PhysicsEuler.h"
#include "Components/ColliderComponent.h"

namespace Game
{
	// Implements the projectiles which can be launched from the ship
	class Projectile : public GameObject
	{
	private:
		std::unique_ptr<Geometry::Mesh> _mesh;
		std::shared_ptr<Shader::Program> _shader;

		bool _launched;
		float _time;
		float _lifetime;
		bool _friendly;

		Components::ColliderComponent _colliderComponent;
		Components::PhysicsEuler _physicsComponent;

	public:
		Projectile(float lifetime, bool friendly);

		// Advances the projectile simulation by the given time step
		void update(float delta);

		// Draws the projectile at the current position
		void draw();

		// See GameObject
		const Components::ColliderComponent* getColliderComponent();

		// See GameObject
		const Components::PhysicsComponent* getPhysicsComponent();

		// Returns true if the projectile is currently in flight 
		bool isLaunched();

		// Launches the projectile from the given position in the given direction
		void shoot(glm::vec2 position, glm::vec2 direction);

		// Reloads the projectile, allowing it to be fired again
		void reload();

		bool isFriendly() { return _friendly; }
	};
}
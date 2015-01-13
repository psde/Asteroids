#pragma once

#include <memory>

#include "Math/Math.h"

#include "Graphics/Geometry/Mesh.h"

#include "GameObject.h"
#include "Components/PhysicsRungeKutta.h"
#include "Components/CollisionComponent.h"
#include "Projectile.h"

namespace Game
{
	// Implements the ship game object, which is controllable by the player
	class Ship : public GameObject
	{
	private:
		Components::CollisionComponent _collisionComponent;
		Components::PhysicsRungeKutta _physicsComponent;

		std::unique_ptr<Graphics::Mesh> _mesh;
		std::unique_ptr<Graphics::Mesh> _rotatedMesh;
		std::shared_ptr<Graphics::Program> _shader;

		float _size;
		float _rotation;
		bool _moving;
		int _rotating;
		float _reloadTime;
		float _invicibility;

		unsigned int _lives;
		unsigned int _score;

		std::vector<std::shared_ptr<Projectile>> _projectiles;

	public:
		// Creates a ship with the given size
		Ship(float size = 27.5f);

		// Tries to shoot a projectile. If succeeded, a valid shared_ptr<Projectile>
		// is returned, nullptr otherwise.
		const std::shared_ptr<Projectile> shoot();

		// Tells the ship to accelerate in the next simulation step
		void accelerate();

		// Tells the ship to rotate in the next simulation step by the given direction.
		// 1 means clockwise, -1 counter-clockwise, all other values are ignored
		void rotate(int rotation);

		// Resets the ship to its inital position, killing its velocity
		void resetPosition();

		// Makes the ship invincible for a short period 
		void makeInvincible();
		
		// Returns true if the ship is currently invincible
		bool isInvincible();

		// Advances the ships simulation by the given time step
		void update(float delta) override;

		// Draws the ship at its current position
		void draw() override;

		// See GameObject
		const Components::CollisionComponent* getCollisionComponent() const override;

		// See GameObject
		const Components::PhysicsComponent* getPhysicsComponent() const override;

		void addScore(unsigned int points);
		unsigned int getScore() { return _score; }

		void removeLive() { _lives--; }
		unsigned int getLives() { return _lives; }
		void resetLives() { _lives = 3; }
	};
}
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
		std::unique_ptr<Graphics::Mesh> m_mesh;
		std::unique_ptr<Graphics::Mesh> m_rotatedMesh;
		std::shared_ptr<Graphics::Program> m_shader;

		float m_size;
		float m_rotation;
		bool m_moving;
		int m_rotating;
		float m_reloadTime;
		float m_invicibility;

		unsigned int m_lives;
		unsigned int m_score;

		std::vector<std::shared_ptr<Projectile>> m_projectiles;

		// Tries to shoot a projectile. If succeeded, a valid shared_ptr<Projectile>
		// is returned, nullptr otherwise.
		const std::shared_ptr<Projectile> shoot();

	public:
		// Creates a ship with the given size
		Ship(float size = 27.5f);

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

		void destroy() override;

		// Handles input. Returns valid shared_ptr<Projectile> if shot was released
		const std::shared_ptr<Projectile> handleInput();

		void addScore(unsigned int points);
		unsigned int score() { return m_score; }

		void removeLive() { m_lives--; }
		unsigned int lives() { return m_lives; }
		void resetLives() { m_lives = 3; }
	};
}

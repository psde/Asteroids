#pragma once

namespace Components
{
	class ColliderComponent;
	class PhysicsComponent;
}

namespace Game
{
	// Base class for all game objects.
	class GameObject
	{
	public:
		// Virtual (but empty) deconstructor
		virtual ~GameObject() {};

		// Advances the game objects simulation by a given time step
		virtual void update(float timeDelta) = 0;

		// Draws the game object
		virtual void draw() = 0;

		// Returns a const pointer to the game objects ColliderComponent, if any. nullptr if none.
		virtual const Components::ColliderComponent* getColliderComponent() = 0;

		// Returns a const pointer to the game objects PhysicsComponent, if any. nullptr if none.
		virtual const Components::PhysicsComponent* getPhysicsComponent() = 0;
	};
}
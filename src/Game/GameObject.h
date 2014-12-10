#pragma once

namespace Components
{
	class CollisionComponent;
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

		// Returns a const pointer to the game objects CollisionComponent, if any. nullptr if none.
		virtual const Components::CollisionComponent* getCollisionComponent() const = 0;

		// Returns a const pointer to the game objects PhysicsComponent, if any. nullptr if none.
		virtual const Components::PhysicsComponent* getPhysicsComponent() const = 0;

		bool collidesWith(const GameObject& other);
		bool collidesWith(const GameObject* other);
	};
}
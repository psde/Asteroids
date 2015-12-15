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
	protected:
		Components::CollisionComponent* _collisionComponent;
		Components::PhysicsComponent* _physicsComponent;

	public:
		GameObject(Components::CollisionComponent* c = nullptr, Components::PhysicsComponent* p = nullptr);

		virtual ~GameObject() = default;

		// Advances the game objects simulation by a given time step
		virtual void update(float timeDelta) = 0;

		// Draws the game object
		virtual void draw() = 0;

		// Returns a const pointer to the game objects CollisionComponent, if any. nullptr if none.
		const Components::CollisionComponent* getCollisionComponent() const;

		// Returns a const pointer to the game objects PhysicsComponent, if any. nullptr if none.
		const Components::PhysicsComponent* getPhysicsComponent() const;

		bool collidesWith(const GameObject& other);
		bool collidesWith(const GameObject* other);
	};
}
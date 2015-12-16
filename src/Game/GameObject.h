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
		Components::CollisionComponent* m_collisionComponent;
		Components::PhysicsComponent* m_physicsComponent;

	public:
		GameObject(Components::CollisionComponent* c, Components::PhysicsComponent* p);

		virtual ~GameObject() = default;

		// Advances the game objects simulation by a given time step
		virtual void update(float timeDelta) = 0;

		// Draws the game object
		virtual void draw() = 0;

		// Returns a const pointer to the game objects CollisionComponent, if any. nullptr if none.
		const Components::CollisionComponent* collisionComponent() const;

		// Returns a const pointer to the game objects PhysicsComponent, if any. nullptr if none.
		const Components::PhysicsComponent* physicsComponent() const;

		virtual void destroy() {}

		bool collidesWith(const GameObject& other);
		bool collidesWith(const GameObject* other);
	};
}
#pragma once

namespace Components
{
	class ColliderComponent;
	class PhysicsComponent;
}

namespace Game
{

	class GameObject
	{
	public:
		virtual ~GameObject() {};
		virtual void update(float timeDelta) = 0;
		virtual void draw() = 0;
		virtual const Components::ColliderComponent* getColliderComponent() = 0;
		virtual const Components::PhysicsComponent* getPhysicsComponent() = 0;
	};
}
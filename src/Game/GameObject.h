#pragma once

namespace Game
{
	class ColliderComponent;
	class PhysicsComponent;

	class GameObject
	{
	public:
		virtual ~GameObject() {};
		virtual void update(float timeDelta) = 0;
		virtual void draw() = 0;
		virtual const ColliderComponent* getColliderComponent() = 0;
		virtual const PhysicsComponent* getPhysicsComponent() = 0;
	};
}
#include "GameObject.h"

#include "Components/CollisionComponent.h"

namespace Game
{
	GameObject::GameObject(Components::CollisionComponent *collision,
						   Components::PhysicsComponent *physics)
		: _collisionComponent(collision)
		, _physicsComponent(physics)
	{

	}

	bool GameObject::collidesWith(const GameObject& other)
	{
		return collidesWith(&other);
	}

	bool GameObject::collidesWith(const GameObject* other)
	{
		if (getCollisionComponent() == nullptr || other->getCollisionComponent() == nullptr)
			return false;

		return getCollisionComponent()->collidesWith(other->getCollisionComponent());
	}

	const Components::CollisionComponent *GameObject::getCollisionComponent() const
	{
		return _collisionComponent;
	}

	const Components::PhysicsComponent *GameObject::getPhysicsComponent() const
	{
		return _physicsComponent;
	}
}
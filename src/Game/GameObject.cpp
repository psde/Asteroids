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
		if (collisionComponent() == nullptr || other->collisionComponent() == nullptr)
			return false;

		return collisionComponent()->collidesWith(other->collisionComponent());
	}

	const Components::CollisionComponent *GameObject::collisionComponent() const
	{
		return _collisionComponent;
	}

	const Components::PhysicsComponent *GameObject::physicsComponent() const
	{
		return _physicsComponent;
	}
}
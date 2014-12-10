#include "GameObject.h"

#include "Components/CollisionComponent.h"

namespace Game
{
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
}
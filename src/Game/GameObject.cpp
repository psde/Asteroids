#include "GameObject.h"

#include "Components/ColliderComponent.h"

namespace Game
{
	bool GameObject::collidesWith(const GameObject& other)
	{
		return collidesWith(&other);
	}

	bool GameObject::collidesWith(const GameObject* other)
	{
		if (getColliderComponent() == nullptr || other->getColliderComponent() == nullptr)
			return false;

		return getColliderComponent()->collidesWith(other->getColliderComponent());
	}
}
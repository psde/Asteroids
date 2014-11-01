#include "GameObject.h"

#include "Components/ColliderComponent.h"

namespace Game
{
	bool GameObject::collidesWith(GameObject& other)
	{
		if(getColliderComponent() == nullptr || other.getColliderComponent() == nullptr)
			return false;

		return getColliderComponent()->collidesWith(other.getColliderComponent());
	}
}
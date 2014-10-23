#include "ColliderComponent.h"

#include <cmath>

namespace Game
{
	ColliderComponent::ColliderComponent()
	{
		_radius = 0;
	}

	bool ColliderComponent::collidesWith(const ColliderComponent *other) const
	{
		float f = std::pow(other->_position.x - _position.x, 2) + std::pow(other->_position.y - _position.y, 2);
		if (std::pow(other->_radius - _radius, 2) <= f && f < std::pow(other->_radius + _radius, 2))
			return true;
		return false;
	}
}
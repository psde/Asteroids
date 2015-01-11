#include "PhysicsEuler.h"

#include <iostream>

namespace Components
{
	PhysicsEuler::PhysicsEuler()
		: PhysicsComponent()
	{

	}

	PhysicsEuler::~PhysicsEuler()
	{

	}

	void PhysicsEuler::reset(Math::vec2 position, Math::vec2 velocity)
	{
		_state.position = position;
		_state.velocity = velocity;
	}

	void PhysicsEuler::update(float timeDelta)
	{
		_state.position += _state.velocity * timeDelta;
		_state.velocity += _acceleration * timeDelta;

		applyTerminalVelocity(_state);
		performWrapAround(_state);
	}
}
#include "PhysicsEuler.h"

#include <iostream>

namespace Components
{
	PhysicsEuler::PhysicsEuler()
		: _acceleration(glm::vec2(0.f))
		, _terminalVelocity(0.f)
	{

	}

	PhysicsEuler::~PhysicsEuler()
	{

	}

	void PhysicsEuler::reset(glm::vec2 position, glm::vec2 velocity)
	{
		_position = position;
		_velocity = velocity;
	}

	void PhysicsEuler::setAcceleration(glm::vec2 acceleration)
	{
		_acceleration = acceleration;
	}

	void PhysicsEuler::update(float timeDelta)
	{
		_position += _velocity * timeDelta;
		_velocity += _acceleration * timeDelta;

		applyTerminalVelocity(_velocity);
		performWrapAround(_position);
	}
}
#include "PhysicsEuler.h"

#include <iostream>

namespace Components
{
	PhysicsEuler::PhysicsEuler()
		: _position(0.f)
		, _velocity(0.f)
		, _acceleration(0.f)
		, _terminalVelocity(0.f)
	{

	}

	PhysicsEuler::~PhysicsEuler()
	{

	}

	void PhysicsEuler::reset(Math::vec2 position, Math::vec2 velocity)
	{
		_position = position;
		_velocity = velocity;
	}

	void PhysicsEuler::setAcceleration(Math::vec2 acceleration)
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
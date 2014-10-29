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

		if (_terminalVelocity > 0.f && (std::abs(_velocity.x) > _terminalVelocity || std::abs(_velocity.y) > _terminalVelocity))
		{
			_velocity = glm::normalize(_velocity) * std::fmin(glm::length(_velocity), _terminalVelocity);
		}

		// Perform wrap-around
		while (_position.x > 800.0f)
		{
			_position.x -= 800.0f;
		}
		while (_position.x < 0.0f)
		{
			_position.x += 800.0f;
		}

		while (_position.y > 600.0f)
		{
			_position.y -= 600.0f;
		}
		while (_position.y < 0.0f)
		{
			_position.y += 600.0f;
		}
	}
}
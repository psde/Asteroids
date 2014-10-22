#include "PhysicsComponent.h"

namespace Game
{
	PhysicsComponent::PhysicsComponent()
	: _velocity(glm::vec2(0.f))
	, _position(glm::vec2(0.f))
	, _maxVelocity(0.f)
	{

	}

	PhysicsComponent::~PhysicsComponent()
	{

	}

	void PhysicsComponent::reset(glm::vec2 position, glm::vec2 velocity)
	{
		_position = position;
		_velocity = velocity;
	}

	void PhysicsComponent::applyImpulse(glm::vec2 impulseVector)
	{
		_velocity += impulseVector;

		if (_maxVelocity > 0.f)
		{
			_velocity = glm::normalize(_velocity) * std::fmin(glm::length(_velocity), _maxVelocity);
		}
	}

	void PhysicsComponent::update(float timeDelta)
	{
		_position += _velocity * timeDelta;

		// Perform wrap-around
		if (_position.x > 800.0f)
		{
			_position.x -= 800.0f;
		}
		if (_position.x < 0.0f)
		{
			_position.x += 800.0f;
		}

		if (_position.y > 600.0f)
		{
			_position.y -= 600.0f;
		}
		if (_position.y < 0.0f)
		{
			_position.y += 600.0f;
		}
	}
}
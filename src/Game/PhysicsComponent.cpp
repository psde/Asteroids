#include "PhysicsComponent.h"

#include <iostream>

namespace Game
{
	PhysicsComponent::PhysicsComponent()
	:  _terminalVelocity(0.f)
	, _acceleration(glm::vec2(0.f))
	{

	}

	PhysicsComponent::~PhysicsComponent()
	{

	}

	void PhysicsComponent::reset(glm::vec2 position, glm::vec2 velocity)
	{
		_state.position = position;
		_state.velocity = velocity;
	}

	void PhysicsComponent::setAcceleration(glm::vec2 acceleration)
	{
		_acceleration = acceleration;
	}

	glm::vec2 PhysicsComponent::acceleration(const PhysicsComponent::State &state)
	{
		return _acceleration;
	}

	PhysicsComponent::State PhysicsComponent::evaluate(const State &initial)
	{
		PhysicsComponent::State output;
		output.position = initial.velocity;
		output.velocity = acceleration(initial);
		return output;
	}

	PhysicsComponent::State PhysicsComponent::evaluate(const State &initial, float dt, const State &d)
	{
		PhysicsComponent::State state;
		state.position = initial.position + d.position*dt;
		state.velocity = initial.velocity + d.velocity*dt;
		PhysicsComponent::State output;
		output.position = state.velocity;
		output.velocity = acceleration(state);
		return output;
	}

	void PhysicsComponent::integrate(State &state, float dt)
	{
		PhysicsComponent::State a = evaluate(state);
		PhysicsComponent::State b = evaluate(state, dt*0.5f, a);
		PhysicsComponent::State c = evaluate(state, dt*0.5f, b);
		PhysicsComponent::State d = evaluate(state, dt, c);

		const glm::vec2 dxdt = 1.0f / 6.0f * (a.position + 2.0f*(b.position + c.position) + d.position);
		const glm::vec2 dvdt = 1.0f / 6.0f * (a.velocity + 2.0f*(b.velocity + c.velocity) + d.velocity);

		state.position = state.position + dxdt*dt;
		state.velocity = state.velocity + dvdt*dt;
	}

	void PhysicsComponent::update(float timeDelta)
	{		
		integrate(_state, timeDelta);

		if (_terminalVelocity > 0.f && (std::abs(_state.velocity.x) > _terminalVelocity || std::abs(_state.velocity.y) > _terminalVelocity))
		{
			_state.velocity = glm::normalize(_state.velocity) * std::fmin(glm::length(_state.velocity), _terminalVelocity);
		}

		// Perform wrap-around
		while (_state.position.x > 800.0f)
		{
			_state.position.x -= 800.0f;
		}
		while (_state.position.x < 0.0f)
		{
			_state.position.x += 800.0f;
		}

		while (_state.position.y > 600.0f)
		{
			_state.position.y -= 600.0f;
		}
		while (_state.position.y < 0.0f)
		{
			_state.position.y += 600.0f;
		}
	}
}
#include "PhysicsComponent.h"

#include <cmath>
#include <iostream>

namespace Components
{
	void PhysicsComponent::performWrapAround(PhysicsState &state)
	{
		// Perform wrap-around
		while (state.position.x > 800.0f)
		{
			state.position.x -= 800.0f;
		}
		while (state.position.x < 0.0f)
		{
			state.position.x += 800.0f;
		}

		while (state.position.y > 600.0f)
		{
			state.position.y -= 600.0f;
		}
		while (state.position.y < 0.0f)
		{
			state.position.y += 600.0f;
		}
	}

	void PhysicsComponent::applyTerminalVelocity(PhysicsState &state)
	{
		float terminalVelocity = getTerminalVelocity();
		if (terminalVelocity > 0.f &&
			(std::abs(state.velocity.x) > terminalVelocity || std::abs(state.velocity.y) > terminalVelocity))
		{
			state.velocity = Math::normalize(state.velocity) * std::fmin(Math::length(state.velocity), terminalVelocity);
		}
	}
}
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
		m_state.position = position;
		m_state.velocity = velocity;
	}

	void PhysicsEuler::update(float timeDelta)
	{
		m_state.position += m_state.velocity * timeDelta;
		m_state.velocity += m_acceleration * timeDelta;

		applyTerminalVelocity(m_state);
		performWrapAround(m_state);
	}
}

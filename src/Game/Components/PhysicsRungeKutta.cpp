#include "PhysicsRungeKutta.h"

#include <iostream>

namespace Components
{
	PhysicsRungeKutta::PhysicsRungeKutta()
		: PhysicsComponent()
	{

	}

	PhysicsRungeKutta::~PhysicsRungeKutta()
	{

	}

	void PhysicsRungeKutta::reset(Math::vec2 position, Math::vec2 velocity)
	{
		m_state.position = position;
		m_state.velocity = velocity;
	}

	PhysicsState PhysicsRungeKutta::evaluate(const PhysicsState &initial)
	{
		PhysicsState output;
		output.position = initial.velocity;
		output.velocity = m_acceleration;
		return output;
	}

	PhysicsState PhysicsRungeKutta::evaluate(const PhysicsState &initial, float dt, const PhysicsState &d)
	{
		PhysicsState state;
		state.position = initial.position + d.position*dt;
		state.velocity = initial.velocity + d.velocity*dt;
		PhysicsState output;
		output.position = state.velocity;
		output.velocity = m_acceleration;
		return output;
	}

	void PhysicsRungeKutta::integrate(PhysicsState &state, float dt)
	{
		PhysicsState a = evaluate(state);
		PhysicsState b = evaluate(state, dt*0.5f, a);
		PhysicsState c = evaluate(state, dt*0.5f, b);
		PhysicsState d = evaluate(state, dt, c);

		const Math::vec2 dxdt = 1.0f / 6.0f * (a.position + 2.0f*(b.position + c.position) + d.position);
		const Math::vec2 dvdt = 1.0f / 6.0f * (a.velocity + 2.0f*(b.velocity + c.velocity) + d.velocity);

		state.position = state.position + dxdt*dt;
		state.velocity = state.velocity + dvdt*dt;
	}

	void PhysicsRungeKutta::update(float timeDelta)
	{
		integrate(m_state, timeDelta);

		applyTerminalVelocity(m_state);
		performWrapAround(m_state);
	}
}
#include "PhysicsRungeKutta.h"

#include <iostream>

namespace Components
{
	PhysicsRungeKutta::PhysicsRungeKutta()
		: _acceleration(0.f)
		, _terminalVelocity(0.f)
	{

	}

	PhysicsRungeKutta::~PhysicsRungeKutta()
	{

	}

	void PhysicsRungeKutta::reset(Math::vec2 position, Math::vec2 velocity)
	{
		_state.position = position;
		_state.velocity = velocity;
	}

	void PhysicsRungeKutta::setAcceleration(Math::vec2 acceleration)
	{
		_acceleration = acceleration;
	}

	Math::vec2 PhysicsRungeKutta::acceleration(const State &/*state*/)
	{
		return _acceleration;
	}

	PhysicsRungeKutta::State PhysicsRungeKutta::evaluate(const State &initial)
	{
		State output;
		output.position = initial.velocity;
		output.velocity = acceleration(initial);
		return output;
	}

	PhysicsRungeKutta::State PhysicsRungeKutta::evaluate(const State &initial, float dt, const State &d)
	{
		State state;
		state.position = initial.position + d.position*dt;
		state.velocity = initial.velocity + d.velocity*dt;
		State output;
		output.position = state.velocity;
		output.velocity = acceleration(state);
		return output;
	}

	void PhysicsRungeKutta::integrate(State &state, float dt)
	{
		State a = evaluate(state);
		State b = evaluate(state, dt*0.5f, a);
		State c = evaluate(state, dt*0.5f, b);
		State d = evaluate(state, dt, c);

		const Math::vec2 dxdt = 1.0f / 6.0f * (a.position + 2.0f*(b.position + c.position) + d.position);
		const Math::vec2 dvdt = 1.0f / 6.0f * (a.velocity + 2.0f*(b.velocity + c.velocity) + d.velocity);

		state.position = state.position + dxdt*dt;
		state.velocity = state.velocity + dvdt*dt;
	}

	void PhysicsRungeKutta::update(float timeDelta)
	{
		integrate(_state, timeDelta);

		applyTerminalVelocity(_state.velocity);
		performWrapAround(_state.position);
	}
}
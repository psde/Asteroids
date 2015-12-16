#pragma once

#include "Math/Math.h"
#include "PhysicsState.h"

namespace Components
{
	// Base class for all physics components, which are responsible for the physics
	// simulation.
	class PhysicsComponent
	{
	protected:

		void performWrapAround(PhysicsState &state);
		void applyTerminalVelocity(PhysicsState &state);

		PhysicsState m_state;
		Math::vec2 m_acceleration;
		float m_terminalVelocity;

	public:
		PhysicsComponent()
			: m_acceleration(0.0f)
			, m_terminalVelocity(0.0f)
		{}

		virtual ~PhysicsComponent() = default;

		// Resets the simulation to given position and velocity
		virtual void reset(Math::vec2 position, Math::vec2 velocity) = 0;

		// Sets the terminal veloicyt to given velocity
		void setTerminalVelocity(float max) { m_terminalVelocity = max; }

		// Returns the terminal velocity
		float getTerminalVelocity() const { return m_terminalVelocity; }

		// Advances the physics simulation by given time delta
		virtual void update(float timeDelta) = 0;

		// Sets the acceleration
		void setAcceleration(Math::vec2 acceleration) { m_acceleration = acceleration; }

		// Returns the current physics state
		const PhysicsState getState() const { return m_state; }

		// Returns the current physics states position
		const Math::vec2 getPosition() const { return m_state.position; }

		// Returns the current physics states velocity
		const Math::vec2 getVelocity() const { return m_state.velocity; }

		// Returns the current acceleration
		const Math::vec2 getAcceleration() const { return m_acceleration; }
	};
}

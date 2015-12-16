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

		PhysicsState _state;
		Math::vec2 _acceleration;
		float _terminalVelocity;

	public:
		PhysicsComponent()
			: _acceleration(0.0f)
			, _terminalVelocity(0.0f)
		{}

		virtual ~PhysicsComponent() = default;

		// Resets the simulation to given position and velocity
		virtual void reset(Math::vec2 position, Math::vec2 velocity) = 0;

		// Sets the terminal veloicyt to given velocity
		void setTerminalVelocity(float max) { _terminalVelocity = max; }

		// Returns the terminal velocity
		float getTerminalVelocity() const { return _terminalVelocity; }

		// Advances the physics simulation by given time delta
		virtual void update(float timeDelta) = 0;

		// Sets the acceleration
		void setAcceleration(Math::vec2 acceleration) { _acceleration = acceleration; }

		// Returns the current physics state
		const PhysicsState getState() const { return _state; }

		// Returns the current physics states position
		const Math::vec2 getPosition() const { return _state.position; }

		// Returns the current physics states velocity
		const Math::vec2 getVelocity() const { return _state.velocity; }

		// Returns the current acceleration
		const Math::vec2 getAcceleration() const { return _acceleration; }
	};
}
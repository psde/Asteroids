#pragma once

#include "Math/Math.h"

namespace Components
{
	// Base class for all physics components, which are responsible for the physics
	// simulation.
	class PhysicsComponent
	{
	protected:

		void performWrapAround(Math::vec2 &position);
		void applyTerminalVelocity(Math::vec2 &velocity);

	public:
		// Virtual (but empty) deconstructor
		virtual ~PhysicsComponent() {};

		// Resets the simulation to given position and velocity
		virtual void reset(Math::vec2 position, Math::vec2 velocity) = 0;

		// Sets the terminal veloicyt to given velocity
		virtual void setTerminalVelocity(float velocity) = 0;

		// Returns the terminal velocity
		virtual float getTerminalVelocity() = 0;

		// Advances the physics simulation by given time delta
		virtual void update(float timeDelta) = 0;

		// Sets the acceleration
		virtual void setAcceleration(Math::vec2 acceleration) = 0;

		// Returns the current position
		virtual const Math::vec2 getPosition() const = 0;

		// Returns the current velocity
		virtual const Math::vec2 getVelocity() const = 0;

		// Returns the current acceleration
		virtual const Math::vec2 getAcceleration() const = 0;
	};
}
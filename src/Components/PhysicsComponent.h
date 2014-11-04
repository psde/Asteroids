#pragma once

//#include <glm/glm.hpp>
#include "Math/Math.h"

namespace Components
{
	// Base class for all physics components, which are responsible for the physics
	// simulation.
	class PhysicsComponent
	{
	protected:

		void performWrapAround(glm::vec2 &position);
		void applyTerminalVelocity(glm::vec2 &velocity);

	public:
		// Virtual (but empty) deconstructor
		virtual ~PhysicsComponent() {};

		// Resets the simulation to given position and velocity
		virtual void reset(glm::vec2 position, glm::vec2 velocity) = 0;

		// Sets the terminal veloicyt to given velocity
		virtual void setTerminalVelocity(float velocity) = 0;

		// Returns the terminal velocity
		virtual float getTerminalVelocity() = 0;

		// Advances the physics simulation by given time delta
		virtual void update(float timeDelta) = 0;

		// Sets the acceleration
		virtual void setAcceleration(glm::vec2 acceleration) = 0;

		// Returns the current position
		virtual const glm::vec2 getPosition() const = 0;

		// Returns the current velocity
		virtual const glm::vec2 getVelocity() const = 0;

		// Returns the current acceleration
		virtual const glm::vec2 getAcceleration() const = 0;
	};
}
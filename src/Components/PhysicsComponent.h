#pragma once

#include <glm/glm.hpp>

namespace Components
{
	class PhysicsComponent
	{
	public:
		virtual ~PhysicsComponent() {};
		virtual void reset(glm::vec2 position, glm::vec2 velocity) = 0;;
		virtual void setTerminalVelocity(float max) = 0;

		virtual void update(float timeDelta) = 0;
		virtual void setAcceleration(glm::vec2 acceleration) = 0;

		virtual const glm::vec2 getPosition() const = 0;
		virtual const glm::vec2 getVelocity() const = 0;
		virtual const glm::vec2 getAcceleration() const = 0;
	};
}
#pragma once

//#include <glm/glm.hpp>
#include "Math/Math.h"

#include "PhysicsComponent.h"

namespace Components
{
	class PhysicsEuler : public PhysicsComponent
	{
	private:
		glm::vec2 _position;
		glm::vec2 _velocity;
		glm::vec2 _acceleration;
		float _terminalVelocity;

	public:
		PhysicsEuler();
		~PhysicsEuler();

		void reset(glm::vec2 position, glm::vec2 velocity);

		void setTerminalVelocity(float max) { _terminalVelocity = max; }
		virtual float getTerminalVelocity() { return _terminalVelocity; }

		void update(float timeDelta);
		void setAcceleration(glm::vec2 acceleration);

		const glm::vec2 getPosition() const { return _position; }
		const glm::vec2 getVelocity() const { return _velocity; }
		const glm::vec2 getAcceleration() const { return _acceleration; }
	};

}
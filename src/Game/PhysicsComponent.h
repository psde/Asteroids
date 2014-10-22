#pragma once

#include <glm/glm.hpp>

namespace Game
{
	class PhysicsComponent
	{
	private:
		glm::vec2 _position;
		glm::vec2 _velocity;
		float _maxVelocity;

	public:
		PhysicsComponent();
		~PhysicsComponent();

		void reset(glm::vec2 position, glm::vec2 velocity);
		void setMaxVelocity(float max) { _maxVelocity = max; }

		void update(float timeDelta);
		void applyImpulse(glm::vec2 impulseVector);

		glm::vec2 getPosition() { return _position; }
		glm::vec2 getVelocity() { return _velocity; }
	};

}
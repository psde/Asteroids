#pragma once

#include <glm/glm.hpp>

namespace Game
{
	class PhysicsComponent
	{
	private:
		struct State {
			State()
			: position(0.f)
			, velocity(0.f)
			{}
			glm::vec2 position, velocity;
		};

		State _state;
		glm::vec2 _acceleration;
		float _terminalVelocity;


		glm::vec2 acceleration(const State &state);
		State evaluate(const State &initial);
		State evaluate(const State &initial, float dt, const State &d);
		void integrate(State &state, float dt);

	public:
		PhysicsComponent();
		~PhysicsComponent();

		void reset(glm::vec2 position, glm::vec2 velocity);
		void setTerminalVelocity(float max) { _terminalVelocity = max; }

		void update(float timeDelta);
		void setAcceleration(glm::vec2 acceleration);

		const glm::vec2 getPosition() const { return _state.position; }
		const glm::vec2 getVelocity() const { return _state.velocity; }
		const glm::vec2 getAcceleration() const { return _acceleration; }
	};

}
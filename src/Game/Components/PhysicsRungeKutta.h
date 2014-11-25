#pragma once

#include "Math/Math.h"

#include "PhysicsComponent.h"

namespace Components
{
	class PhysicsRungeKutta : public PhysicsComponent
	{
	private:
		struct State {
			State()
			: position(0.f)
			, velocity(0.f)
			{}
			Math::vec2 position, velocity;
		};

		State _state;
		Math::vec2 _acceleration;
		float _terminalVelocity;

		Math::vec2 acceleration(const State &state);
		State evaluate(const State &initial);
		State evaluate(const State &initial, float dt, const State &d);
		void integrate(State &state, float dt);

	public:
		PhysicsRungeKutta();
		~PhysicsRungeKutta();

		void reset(Math::vec2 position, Math::vec2 velocity);

		void setTerminalVelocity(float max) { _terminalVelocity = max; }
		virtual float getTerminalVelocity() { return _terminalVelocity; }

		void update(float timeDelta);
		void setAcceleration(Math::vec2 acceleration);

		const Math::vec2 getPosition() const { return _state.position; }
		const Math::vec2 getVelocity() const { return _state.velocity; }
		const Math::vec2 getAcceleration() const { return _acceleration; }
	};

}
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

		void reset(Math::vec2 position, Math::vec2 velocity) override;

		void setTerminalVelocity(float max) override { _terminalVelocity = max; }
		virtual float getTerminalVelocity() override { return _terminalVelocity; }

		void update(float timeDelta) override;
		void setAcceleration(Math::vec2 acceleration) override;

		const Math::vec2 getPosition() const override { return _state.position; }
		const Math::vec2 getVelocity() const override { return _state.velocity; }
		const Math::vec2 getAcceleration() const override { return _acceleration; }
	};

}
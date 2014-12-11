#pragma once

#include "Math/Math.h"

#include "PhysicsComponent.h"

namespace Components
{
	class PhysicsEuler : public PhysicsComponent
	{
	private:
		Math::vec2 _position;
		Math::vec2 _velocity;
		Math::vec2 _acceleration;
		float _terminalVelocity;

	public:
		PhysicsEuler();
		~PhysicsEuler();

		void reset(Math::vec2 position, Math::vec2 velocity) override;

		void setTerminalVelocity(float max) override { _terminalVelocity = max; }
		virtual float getTerminalVelocity() const override { return _terminalVelocity; }

		void update(float timeDelta) override;
		void setAcceleration(Math::vec2 acceleration) override;

		const Math::vec2 getPosition() const override { return _position; }
		const Math::vec2 getVelocity() const override { return _velocity; }
		const Math::vec2 getAcceleration() const override { return _acceleration; }
	};

}
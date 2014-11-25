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

		void reset(Math::vec2 position, Math::vec2 velocity);

		void setTerminalVelocity(float max) { _terminalVelocity = max; }
		virtual float getTerminalVelocity() { return _terminalVelocity; }

		void update(float timeDelta);
		void setAcceleration(Math::vec2 acceleration);

		const Math::vec2 getPosition() const { return _position; }
		const Math::vec2 getVelocity() const { return _velocity; }
		const Math::vec2 getAcceleration() const { return _acceleration; }
	};

}
#pragma once

#include "Math/Math.h"

#include "PhysicsComponent.h"

namespace Components
{
	class PhysicsRungeKutta : public PhysicsComponent
	{
	private:
		PhysicsState evaluate(const PhysicsState &initial);
		PhysicsState evaluate(const PhysicsState &initial, float dt, const PhysicsState &d);
		void integrate(PhysicsState &state, float dt);

	public:
		PhysicsRungeKutta();
		~PhysicsRungeKutta();

		void reset(Math::vec2 position, Math::vec2 velocity) override;

		void update(float timeDelta) override;
	};

}

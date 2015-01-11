#pragma once

#include "Math/Math.h"

#include "PhysicsComponent.h"

namespace Components
{
	class PhysicsEuler : public PhysicsComponent
	{
	private:

	public:
		PhysicsEuler();
		~PhysicsEuler();

		void reset(Math::vec2 position, Math::vec2 velocity) override;

		void update(float timeDelta) override;
	};

}
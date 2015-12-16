#pragma once

#include "Math/Math.h"

namespace Components
{
	struct PhysicsState
	{
		PhysicsState() 
			: position(0), velocity(0) {}

		PhysicsState(Math::vec2 pos, Math::vec2 vel)
			: position(pos), velocity(vel) {}

		Math::vec2 position;
		Math::vec2 velocity;
	};
}

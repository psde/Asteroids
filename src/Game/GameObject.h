#pragma once

namespace Game
{
	class GameObject
	{
	public:
		virtual void update(float timeDelta) = 0;
		virtual void draw() = 0;
		//virtual bool collidesWith(const GameObject& other) = 0;
	};
}
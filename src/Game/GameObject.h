#pragma once

namespace Game
{
	class ColliderComponent;

	class GameObject
	{
	public:
		virtual void update(float timeDelta) = 0;
		virtual void draw() = 0;
		virtual const ColliderComponent* getColliderComponent() = 0;
	};
}
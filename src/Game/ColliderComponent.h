#pragma once

#include <glm/glm.hpp>

namespace Game
{
	class ColliderComponent
	{
	private:
		float _radius;
		glm::vec2 _position;

	public:
		ColliderComponent();

		void setRadius(float r) { _radius = r; }
		void setPosition(glm::vec2 pos) { _position = pos; }

		bool collidesWith(const ColliderComponent *other) const;
	};
}
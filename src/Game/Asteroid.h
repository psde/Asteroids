#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Geometry\Mesh.h"

namespace Game
{
	class Asteroid
	{
	private:
		int _size;

		glm::vec2 _position;
		glm::vec2 _velocity;
		float _rotation;

		std::unique_ptr<Geometry::Mesh> _mesh;
		Shader::Program _shader;

	public:
		Asteroid(int size);

		void draw();

		void update(float delta);
	};

}
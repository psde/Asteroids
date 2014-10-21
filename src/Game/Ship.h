#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Geometry/Mesh.h"

namespace Game
{
	class Ship
	{
	private:
		int _size;

		glm::vec2 _position;
		glm::vec2 _velocity;
		float _rotation;
		bool _rotationDirty;
		bool _moving;

		std::unique_ptr<Geometry::Mesh> _mesh;
		std::unique_ptr<Geometry::Mesh> _rotatedMesh;
		std::shared_ptr<Shader::Program> _shader;

	public:
		Ship();

		void draw();

		void update(bool moving, float delta);
	};
}
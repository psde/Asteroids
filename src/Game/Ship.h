#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Geometry/Mesh.h"

#include "GameObject.h"
#include "PhysicsComponent.h"

namespace Game
{
	class Ship// : public GameObject
	{
	private:
		int _size;

		PhysicsComponent _physicsComponent;

		float _rotation;
		float _acceleration;
		bool _rotationDirty;
		bool _moving;
		
		std::unique_ptr<Geometry::Mesh> _mesh;
		std::unique_ptr<Geometry::Mesh> _rotatedMesh;
		std::shared_ptr<Shader::Program> _shader;

	public:
		Ship();

		void draw();

		void update(bool moving, int rotation, float delta);
	};
}
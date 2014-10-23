#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Geometry/Mesh.h"

#include "GameObject.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"

namespace Game
{
	class Asteroid : public GameObject
	{
	private:
		int _size;

		PhysicsComponent _physicsComponent;
		ColliderComponent _colliderComponent;
		float _rotation;

		std::unique_ptr<Geometry::Mesh> _mesh;
		std::shared_ptr<Shader::Program> _shader;
	public:
		Asteroid(int size);

		void draw();

		void update(float delta);

		const ColliderComponent* getColliderComponent();
	};

}
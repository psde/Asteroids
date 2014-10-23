#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Geometry/Mesh.h"

#include "GameObject.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"

namespace Game
{
	class Ship : public GameObject
	{
	private:
		int _size;

		ColliderComponent _colliderComponent;

		float _rotation;
		float _acceleration;
		bool _moving;
		int _rotating;
		
		std::unique_ptr<Geometry::Mesh> _mesh;
		std::unique_ptr<Geometry::Mesh> _rotatedMesh;
		std::shared_ptr<Shader::Program> _shader;

	public:
		PhysicsComponent _physicsComponent;

		Ship();

		void accelerate();
		void rotate(int rotation);

		void update(float delta);

		void draw();

		const ColliderComponent* getColliderComponent();
	};
}
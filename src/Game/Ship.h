#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Geometry/Mesh.h"

#include "GameObject.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"
#include "Projectile.h"

namespace Game
{
	class Ship : public GameObject
	{
	private:
		int _size;

		ColliderComponent _colliderComponent;
		PhysicsComponent _physicsComponent;

		float _rotation;
		bool _moving;
		int _rotating;
		float _reloadTime;
		
		std::unique_ptr<Geometry::Mesh> _mesh;
		std::unique_ptr<Geometry::Mesh> _rotatedMesh;
		std::shared_ptr<Shader::Program> _shader;

		std::vector<std::shared_ptr<Projectile>> _projectiles;

	public:
		Ship(float size = 27.5f);

		const std::shared_ptr<Projectile> shoot();
		void accelerate();
		void rotate(int rotation);

		void update(float delta);

		void draw();

		const ColliderComponent* getColliderComponent();
		const PhysicsComponent* getPhysicsComponent();
	};
}
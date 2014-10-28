#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Geometry/Mesh.h"

#include "GameObject.h"
#include "PhysicsComponent.h"
#include "ColliderComponent.h"

namespace Game
{
	class Projectile : public GameObject
	{
	private:
		bool _launched;
		float _time;

		ColliderComponent _colliderComponent;
		PhysicsComponent _physicsComponent;

		std::unique_ptr<Geometry::Mesh> _mesh;
		std::shared_ptr<Shader::Program> _shader;

	public:
		Projectile();
		
		void update(float delta);

		void draw();

		const ColliderComponent* getColliderComponent();
		const PhysicsComponent* getPhysicsComponent();

		bool isLaunched();
		void shoot(glm::vec2 position, glm::vec2 direction);
		void reload();
	};
}
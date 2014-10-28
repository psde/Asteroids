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
	public:
		static const std::vector<float> &AsteroidSizes()
		{
			static std::vector <float> sizes = { 15.f, 45.f, 95.f };
			return sizes;
		}

	private:
		int _size;

		PhysicsComponent _physicsComponent;
		ColliderComponent _colliderComponent;
		float _rotation;

		std::unique_ptr<Geometry::Mesh> _mesh;
		std::shared_ptr<Shader::Program> _shader;

		void init(int size, glm::vec2 position, glm::vec2 direction);
	public:
		Asteroid(int size);
		Asteroid(int size, glm::vec2 position, glm::vec2 direction);

		void draw();

		void update(float delta);

		const ColliderComponent* getColliderComponent();
		const PhysicsComponent* getPhysicsComponent();

		int getAsteroidSize();
	};

}
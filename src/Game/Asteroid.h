#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Geometry/Mesh.h"

#include "GameObject.h"
#include "Components/PhysicsEuler.h"
#include "Components/ColliderComponent.h"

namespace Game
{
	// Implements the asteroid game object, which will float in space with a 
	// random velocity and direction, but with no acceleration.
	class Asteroid : public GameObject
	{
	private:
		int _size;
		float _meshSize;

		Components::PhysicsEuler _physicsComponent;
		Components::ColliderComponent _colliderComponent;
		float _rotation;

		std::unique_ptr<Geometry::Mesh> _mesh;
		std::shared_ptr<Shader::Program> _shader;

		void init(int size, glm::vec2 position, glm::vec2 direction);
	public:
		// Creates an asteroid with a random position and random velocity, 
		// and with a size which is determined by the given size index
		Asteroid(int size);

		// Creates an asteroid with a given position and given direction, 
		// but random velocity depending on the asteroid size
		// and with a size which is determined by the given size index
		Asteroid(int size, glm::vec2 position, glm::vec2 direction);

		// Draws the asteroid at the current position
		void draw();

		// Advances the asteroids simulation by the given time delta
		void update(float delta);

		// See GameObject
		const Components::ColliderComponent* getColliderComponent();

		// See GameObject
		const Components::PhysicsComponent* getPhysicsComponent();

		// Returns the current asteroid size index
		int getAsteroidSize();

		// Returns a vector<float> containt all possible asteroid sizes, 
		// accessible with the size index
		static const std::vector<float> &AsteroidSizes()
		{
			static std::vector <float> sizes = { 15.f, 45.f, 95.f };
			return sizes;
		}
	};

}
#pragma once

#include <memory>

#include "Math/Math.h"

#include "Graphics/Geometry/Mesh.h"

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
		bool _destroyed;

		Components::PhysicsEuler _physicsComponent;
		Components::ColliderComponent _colliderComponent;
		float _rotation;

		std::unique_ptr<Geometry::Mesh> _mesh;
		std::shared_ptr<Shader::Program> _shader;

		void init(int size, Math::vec2 position, Math::vec2 direction);
	public:
		// Creates an asteroid with a random position and random velocity, 
		// and with maximum size.
		Asteroid();

		// Creates an asteroid with a random position and random velocity, 
		// and with a size which is determined by the given size index
		Asteroid(int size);

		// Creates an asteroid with a given position and given direction, 
		// but random velocity depending on the asteroid size
		// and with a size which is determined by the given size index
		Asteroid(int size, Math::vec2 position, Math::vec2 direction);

		// Draws the asteroid at the current position
		void draw();

		// Advances the asteroids simulation by the given time delta
		void update(float delta);

		// See GameObject
		const Components::ColliderComponent* getColliderComponent() const;

		// See GameObject
		const Components::PhysicsComponent* getPhysicsComponent() const;

		// Returns the current asteroid size index
		int getAsteroidSize();

		void destroy() { _destroyed = true; }
		bool isDestroyed() { return _destroyed; }

		// Returns a vector<float> containt all possible asteroid sizes, 
		// accessible with the size index
		static const std::vector<float> &AsteroidSizes()
		{
			static std::vector <float> sizes = { 15.f, 45.f, 95.f };
			return sizes;
		}
	};

}
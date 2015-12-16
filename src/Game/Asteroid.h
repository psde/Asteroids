#pragma once

#include <memory>

#include "Math/Math.h"

#include "Graphics/Geometry/Mesh.h"

#include "GameObject.h"
#include "Components/PhysicsEuler.h"
#include "Components/CollisionComponent.h"

namespace Game
{
	// Implements the asteroid game object, which will float in space with a 
	// random velocity and direction, but with no acceleration.
	class Asteroid : public GameObject
	{
	private:
		int m_size;
		float m_meshSize;
		bool m_destroyed;

		float m_rotation;

		std::unique_ptr<Graphics::Mesh> m_mesh;
		std::shared_ptr<Graphics::Program> m_shader;

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
		void draw() override;

		// Advances the asteroids simulation by the given time delta
		void update(float delta) override;

		// Returns the current asteroid size index
		int asteroidSize();

		void destroy() override;
		bool isDestroyed() { return m_destroyed; }

		// Returns a vector<float> contains all possible asteroid sizes,
		// accessible with the size index
		static const std::vector<float> &AsteroidSizes()
		{
			static std::vector <float> sizes = { 12.f, 35.f, 65.f, 100.f };
			return sizes;
		}
	};

}
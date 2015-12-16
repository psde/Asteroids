#include <random>
#include <iostream>

#include <memory>
#include "Math/Math.h"

#include "ParticleEmitter.h"

#include "Asteroid.h"

namespace Game
{
	std::unique_ptr<Graphics::Mesh> generateAsteroid(float size)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> dis(1, 3);
		std::uniform_real_distribution<float> offset(-0.025f, 0.025f);

		int type = dis(gen);

		std::vector<Math::vec2> vertices;
		switch (type)
		{
		default:
		case 1:
			vertices = {
				Math::vec2(0.27f, 0.08f),
				Math::vec2(0.82f, 0.10f),
				Math::vec2(0.66f, 0.30f),
				Math::vec2(0.82f, 0.56f),
				Math::vec2(0.60f, 0.81f),
				Math::vec2(0.54f, 0.63f),
				Math::vec2(0.31f, 0.76f),
				Math::vec2(0.10f, 0.79f),
				Math::vec2(0.05f, 0.40f),
				Math::vec2(0.38f, 0.28f)
			};
			break;
		case 2:
			vertices = {
				Math::vec2(0.30f, 0.08f),
				Math::vec2(0.69f, 0.09f),
				Math::vec2(0.83f, 0.29f),
				Math::vec2(0.83f, 0.58f),
				Math::vec2(0.70f, 0.82f),
				Math::vec2(0.51f, 0.82f),
				Math::vec2(0.53f, 0.60f),
				Math::vec2(0.26f, 0.76f),
				Math::vec2(0.18f, 0.62f),
				Math::vec2(0.43f, 0.53f),
				Math::vec2(0.10f, 0.35f)
			};
			break;
		case 3:
			vertices = {
				Math::vec2(0.27f, 0.14f),
				Math::vec2(0.45f, 0.28f),
				Math::vec2(0.77f, 0.22f),
				Math::vec2(0.89f, 0.38f),
				Math::vec2(0.65f, 0.46f),
				Math::vec2(0.82f, 0.59f),
				Math::vec2(0.70f, 0.76f),
				Math::vec2(0.43f, 0.71f),
				Math::vec2(0.32f, 0.79f),
				Math::vec2(0.13f, 0.64f),
				Math::vec2(0.25f, 0.54f),
				Math::vec2(0.23f, 0.45f),
				Math::vec2(0.08f, 0.35f)
			};
			break;
		case 4:
			vertices = {
				Math::vec2(0.25f, 0.25f),
				Math::vec2(0.75f, 0.25f),
				Math::vec2(0.75f, 0.75f),
				Math::vec2(0.25f, 0.75f)
			};
			break;
		}

		for (auto &v : vertices)
		{
			v -= Math::vec2(0.5f);
			v += Math::vec2(offset(gen), offset(gen));
			//v = Math::rotate(v, rotation);
			v += Math::vec2(0.5f);
			v *= size;
		}

		return std::unique_ptr<Graphics::Mesh>(new Graphics::Mesh(vertices));
	}

	Asteroid::Asteroid()
		: Asteroid(Asteroid::AsteroidSizes().size() - 1)
	{}

	Asteroid::Asteroid(int size)
		: GameObject(new Components::CollisionComponent(), new Components::PhysicsEuler())
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis_x(0, 800);
		std::uniform_real_distribution<float> dis_y(0, 600);
		std::uniform_real_distribution<float> dir(-1, 1);

		Math::vec2 position = Math::vec2(dis_x(gen), dis_y(gen));
		Math::vec2 direction = Math::vec2(dir(gen), dir(gen));

		if (position.x > 200 && position.x < 450 && 
			position.y > 200 && position.y < 350)
		{
			position.x += 200;
			position.y += 200;
		}

		init(size, position, direction);
	}

	Asteroid::Asteroid(int size, Math::vec2 position, Math::vec2 direction)
		: GameObject(new Components::CollisionComponent(), new Components::PhysicsEuler())
	{
		init(size, position, direction);
	}

	void Asteroid::init(int size, Math::vec2 position, Math::vec2 direction)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> vel(30.0, 55.0);
		std::exponential_distribution<float> vel2(50);
		std::uniform_real_distribution<float> rotation(0, 2.f * Math::pi<float>());
		std::uniform_real_distribution<float> sizeOffset(0.f, 4.f);

		m_destroyed = false;
		m_size = size;
		m_shader = Graphics::Program::getProgram("data/shader/entity.glsl");
		m_rotation = rotation(gen);

		Math::vec2 velocity = direction * Math::vec2(vel2(gen) + 30.f);

		velocity *= (Asteroid::AsteroidSizes().size() - static_cast<float>(size));

		m_physicsComponent->reset(position, velocity);

		m_meshSize = AsteroidSizes().at(m_size) + sizeOffset(gen);
		m_mesh = generateAsteroid(m_meshSize);
		m_mesh = std::move(m_mesh->rotate(m_rotation, Math::vec2(m_meshSize / 2.0f)));

		std::unique_ptr<Graphics::Mesh> collision(new Graphics::Mesh(*m_mesh.get()));
		m_collisionComponent->setCollisionMesh(m_mesh.get());
	}

	void Asteroid::update(float delta)
	{
		m_physicsComponent->update(delta);

		m_collisionComponent->setPosition(m_physicsComponent->getPosition());
	}

	void Asteroid::draw()
	{
		m_shader->use();
		m_shader->uniform("size") = 1.0f;

		for (int y = -1; y <= 1; ++y)
		{
			for (int x = -1; x <= 1; ++x)
			{
				m_shader->uniform("position") = m_physicsComponent->getPosition() + Math::vec2(800 * x, 600 * y);
				m_mesh->draw(Graphics::DrawMode::LineLoop);
			}
		}
	}

	int Asteroid::asteroidSize()
	{
		return m_size;
	}

	void Asteroid::destroy()
	{
		m_destroyed = true;
		float size = Asteroid::AsteroidSizes().at(asteroidSize());
		auto pos = physicsComponent()->getPosition() + (size / 2.f);

		// Emit particle cloud
		ParticleEmitter::instance().emitParticles(pos, size / 2.f, static_cast<int>(size));
	}
}

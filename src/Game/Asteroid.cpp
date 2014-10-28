#include <random>
#include <iostream>

#include <memory>
#include <glm/gtx/rotate_vector.hpp>

#include "Asteroid.h"

namespace Game
{
	namespace
	{
		std::unique_ptr<Geometry::Mesh> generateAsteroid(float size, float rotation)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(1, 3);
			std::uniform_real_distribution<> offset(-0.025, 0.025);

			int type = dis(gen);
			//type = 4;

			std::vector<glm::vec2> vertices;
			switch (type)
			{
			default:
			case 1:
				vertices = {
					glm::vec2(0.27f, 0.08f),
					glm::vec2(0.82f, 0.10f),
					glm::vec2(0.66f, 0.30f),
					glm::vec2(0.82f, 0.56f),
					glm::vec2(0.60f, 0.81f),
					glm::vec2(0.54f, 0.63f),
					glm::vec2(0.31f, 0.76f),
					glm::vec2(0.10f, 0.79f),
					glm::vec2(0.05f, 0.40f),
					glm::vec2(0.38f, 0.28f)
				};
				break;
			case 2:
				vertices = {
					glm::vec2(0.30f, 0.08f),
					glm::vec2(0.69f, 0.09f),
					glm::vec2(0.83f, 0.29f),
					glm::vec2(0.83f, 0.58f),
					glm::vec2(0.70f, 0.82f),
					glm::vec2(0.51f, 0.82f),
					glm::vec2(0.53f, 0.60f),
					glm::vec2(0.26f, 0.76f),
					glm::vec2(0.18f, 0.62f),
					glm::vec2(0.43f, 0.53f),
					glm::vec2(0.10f, 0.35f)
				};
				break;
			case 3:
				vertices = {
					glm::vec2(0.27f, 0.14f),
					glm::vec2(0.45f, 0.28f),
					glm::vec2(0.77f, 0.22f),
					glm::vec2(0.89f, 0.38f),
					glm::vec2(0.65f, 0.46f),
					glm::vec2(0.82f, 0.59f),
					glm::vec2(0.70f, 0.76f),
					glm::vec2(0.43f, 0.71f),
					glm::vec2(0.32f, 0.79f),
					glm::vec2(0.13f, 0.64f),
					glm::vec2(0.25f, 0.54f),
					glm::vec2(0.23f, 0.45f),
					glm::vec2(0.08f, 0.35f)
				};
				break;
			case 4:
				vertices = {
					glm::vec2(0.25f, 0.25f),
					glm::vec2(0.75f, 0.25f),
					glm::vec2(0.75f, 0.75f),
					glm::vec2(0.25f, 0.75f)
				};
				break;
			}

			for (auto &v : vertices)
			{
				v -= glm::vec2(0.5f);
				v += glm::vec2(offset(gen), offset(gen));
				v = glm::rotate(v, rotation);
				v += glm::vec2(0.5f);
				v *= size;
			}

			return std::unique_ptr<Geometry::Mesh>(new Geometry::Mesh(vertices));
		}
	}

	Asteroid::Asteroid(int size)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis_x(0, 600);
		std::uniform_real_distribution<> dis_y(0, 400);
		std::uniform_real_distribution<> dir(-1, 1);

		glm::vec2 position = glm::vec2(dis_x(gen), dis_y(gen));
		glm::vec2 direction = glm::vec2(dir(gen), dir(gen));

		if (position.x > 300)
			position.x += 200;

		if (position.y > 200)
			position.y += 200;

		init(size, position, direction);
	}

	Asteroid::Asteroid(int size, glm::vec2 position, glm::vec2 direction)
	{
		init(size, position, direction);
	}

	void Asteroid::init(int size, glm::vec2 position, glm::vec2 direction)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> vel(30.0, 55.0);
		std::exponential_distribution<> vel2(50);
		std::uniform_real_distribution<> rotation(0, 2.f * glm::pi<float>());
		std::uniform_real_distribution<> sizeOffset(0.f, 4.f);

		_size = size;
		_shader = Shader::Program::getProgram("data/shader/asteroid.glsl");
		_rotation = rotation(gen);

		glm::vec2 velocity = direction * glm::vec2(vel2(gen) + 30.f);

		velocity *= (Asteroid::AsteroidSizes().size() - size);

		_physicsComponent.reset(position, velocity);

		_mesh = generateAsteroid(AsteroidSizes().at(_size) + sizeOffset(gen), _rotation);

		std::unique_ptr<Geometry::Mesh> collision(new Geometry::Mesh(*_mesh.get()));
		_colliderComponent.setCollisionMesh(std::move(collision));
	}

	void Asteroid::update(float delta)
	{
		_physicsComponent.update(delta);

		_colliderComponent.setPosition(_physicsComponent.getPosition());
	}

	void Asteroid::draw()
	{
		_shader->use();

		for (int y = -1; y < 1; ++y)
		{
			for (int x = -1; x < 1; ++x)
			{
				_shader->uniform("position") = _physicsComponent.getPosition() + glm::vec2(800 * x, 600 * y);
				_mesh->draw(GL_LINE_LOOP);
			}
		}
	}

	int Asteroid::getAsteroidSize()
	{
		return _size;
	}

	const ColliderComponent* Asteroid::getColliderComponent()
	{
		return &_colliderComponent;
	}

	const PhysicsComponent* Asteroid::getPhysicsComponent()
	{
		return &_physicsComponent;
	}
}
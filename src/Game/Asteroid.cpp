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
			std::uniform_real_distribution<> offset(-0.05, 0.05);

			int type = dis(gen);

			std::vector<GLfloat> vertices;
			switch (type)
			{
			default:
			case 1:
				vertices = {
					0.27f, 0.08f,
					0.82f, 0.10f,
					0.66f, 0.30f,
					0.82f, 0.56f,
					0.60f, 0.81f,
					0.54f, 0.63f,
					0.31f, 0.76f,
					0.10f, 0.79f,
					0.05f, 0.40f,
					0.38f, 0.28f
				};
				break;
			case 2:
				vertices = {
					0.30f, 0.08f,
					0.69f, 0.09f,
					0.83f, 0.29f,
					0.83f, 0.58f,
					0.70f, 0.82f,
					0.51f, 0.82f,
					0.53f, 0.60f,
					0.26f, 0.76f,
					0.18f, 0.62f,
					0.43f, 0.53f,
					0.10f, 0.35f
				};
				break;			
			case 3:
				vertices = {
					0.27f, 0.14f,
					0.45f, 0.28f,
					0.77f, 0.22f,
					0.89f, 0.38f,
					0.65f, 0.46f,
					0.82f, 0.59f,
					0.70f, 0.76f,
					0.43f, 0.71f,
					0.32f, 0.79f,
					0.13f, 0.64f,
					0.25f, 0.54f,
					0.23f, 0.45f,
					0.08f, 0.35f
				};
				break;
			}

			for (decltype(vertices)::size_type i = 0; i < vertices.size(); i+=2)
			{
				glm::vec2 v(vertices[i] - 0.5f, vertices[i + 1] - 0.5f);
				v += glm::vec2(offset(gen), offset(gen));
				v = glm::rotate(v, rotation);
				vertices[i] = (v.x + 0.5f) * size;
				vertices[i + 1] = (v.y + 0.5f) * size;
			}

			std::unique_ptr<Geometry::Mesh> mesh(new Geometry::Mesh(vertices));
			return std::move(mesh);
		}
	}


	Asteroid::Asteroid(int size)
	: _size(size)
	{

		_shader.addUniversalShader("data/shader/asteroid.glsl");

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis_x(0, 800);
		std::uniform_real_distribution<> dis_y(0, 600);
		std::uniform_real_distribution<> vel(30.0, 55.0);
		std::uniform_int_distribution<> vel_sign(0, 1);
		std::uniform_real_distribution<> rotation(0, 2.f * glm::pi<float>());

		float foo[] = { -1.f, 1.f };

		_position = glm::vec2(dis_x(gen), dis_y(gen));
		_velocity = glm::vec2(vel(gen) * foo[vel_sign(gen)], vel(gen) * foo[vel_sign(gen)]);
		_rotation = rotation(gen);

		_mesh = generateAsteroid(_size, _rotation);

	}

	void Asteroid::update(float delta)
	{
		_position += _velocity * delta;

		if (_position.x > 800.0f)
		{
			_position.x -= 800.0f;
		}
		if (_position.x < 0.0f)
		{
			_position.x += 800.0f;
		}

		if (_position.y > 600.0f)
		{
			_position.y -= 600.0f;
		}
		if (_position.y < 0.0f)
		{
			_position.y += 600.0f;
		}
	}

	void Asteroid::draw()
	{
		_shader.use();

		for (int y = -1; y < 1; ++y)
		for (int x = -1; x < 1; ++x)
		{
			_shader["position"] = _position + glm::vec2(800 * x, 600 * y);
			_mesh->draw(GL_LINE_LOOP);
		}
	}
}
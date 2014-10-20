#include <random>
#include <iostream>

#include <glm/gtx/rotate_vector.hpp>

#include "Shader/Manager.h"

#include "Ship.h"

namespace Game
{
	Ship::Ship()
	: _shader(Shader::Manager::getProgram("data/shader/asteroid.glsl"))
	{
		_size = 25.f;
		_position = glm::vec2(400.0f - _size / 2.f, 300 - _size / 2.f);
		_velocity = glm::vec2(0, 0);
		_rotation = 0.0f;

		std::vector<GLfloat> vertices = {
			0.50f * _size, 0.05f * _size,
			0.10f * _size, 0.90f * _size,
			0.90f * _size, 0.90f * _size,
			0.21f * _size, 0.67f * _size,
			0.79f * _size, 0.67f * _size,
			0.50f * _size, 1.00f * _size
		};

		std::vector<GLuint> elements = {
			1, 0, 2, 4, 3, 5, 4
		};

		_mesh.reset(new Geometry::Mesh(vertices, elements));
	}

	void Ship::update(float delta)
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

	void Ship::draw()
	{
		_shader->use();

		for (int y = -1; y < 1; ++y)
		for (int x = -1; x < 1; ++x)
		{
			_shader->uniform("position") = _position + glm::vec2(800 * x, 600 * y);
			_mesh->draw(GL_LINE_STRIP);
		}
	}
}
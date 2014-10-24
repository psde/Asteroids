#include <random>
#include <iostream>

#include <glm/gtx/rotate_vector.hpp>

#include "Shader/Manager.h"

#include "Ship.h"

#include <GLFW/glfw3.h>

namespace Game
{
	Ship::Ship(float size)
	: _shader(Shader::Manager::getProgram("data/shader/ship.glsl"))
	{
		_acceleration = 0.f;
		_moving = false;
		_size = size;
		_rotation = 0.0f;
		_rotating = 0;

		_physicsComponent.reset(glm::vec2(400.0f - _size / 2.f, 300 - _size / 2.f), glm::vec2(0.f));
		_physicsComponent.setMaxVelocity(500.f);

		std::vector<glm::vec2> vertices = {
			glm::vec2(0.50f * _size, 0.05f * _size),
			glm::vec2(0.10f * _size, 0.90f * _size),
			glm::vec2(0.90f * _size, 0.90f * _size),
			glm::vec2(0.21f * _size, 0.67f * _size),
			glm::vec2(0.79f * _size, 0.67f * _size),
			glm::vec2(0.50f * _size, 1.00f * _size)
		};

		std::vector<GLuint> elements = {
			1, 0, 2, 4, 3, 4, 5, 3
		}; 

		_mesh.reset(new Geometry::Mesh(vertices, elements));
		_rotatedMesh = std::unique_ptr<Geometry::Mesh>(new Geometry::Mesh(*_mesh.get()));
	}

	void Ship::accelerate()
	{
		_moving = true;
	}

	void Ship::rotate(int rotation)
	{
		_rotating = rotation;
	}

	void Ship::update(float delta)
	{
		if (_moving)
		{
			_acceleration += 0.5 * delta;
			if (_acceleration > 0.15f)
				_acceleration = 0.15f;

			_physicsComponent.applyImpulse(glm::rotate(glm::vec2(0.f, -_acceleration), _rotation));
		}
		else
		{
			_acceleration = 0.f;
		}

		_physicsComponent.update(delta);

		if (_rotating != 0)
		{
			_rotation += 4.5f * (float)_rotating * delta;
			_rotatedMesh = std::move(_mesh->rotate(_rotation, glm::vec2(_size / 2.0f)));
			_rotating = 0;
		}

		_colliderComponent.setCollisionMesh(std::unique_ptr<Geometry::Mesh>(new Geometry::Mesh(*_rotatedMesh.get())));

		_colliderComponent.setPosition(_physicsComponent.getPosition());
	}

	void Ship::draw()
	{
		_shader->use();

		for (int y = -1; y < 1; ++y)
		for (int x = -1; x < 1; ++x)
		{
			_shader->uniform("position") = _physicsComponent.getPosition() + glm::vec2(800 * x, 600 * y);
			_rotatedMesh->draw(GL_LINE_STRIP, 5, 0);
			if (_moving && std::fmod(glfwGetTime(), 0.2) >= 0.1)
			{
				_rotatedMesh->draw(GL_LINE_STRIP, 3, 5);
			}
		}

		_moving = false;
	}

	const ColliderComponent* Ship::getColliderComponent()
	{
		return &_colliderComponent;
	}

	const PhysicsComponent* Ship::getPhysicsComponent()
	{
		return &_physicsComponent;
	}
}
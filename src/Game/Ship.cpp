#include <random>
#include <iostream>

#include <glm/gtx/rotate_vector.hpp>

#include "Ship.h"

#include <GLFW/glfw3.h>

namespace Game
{
	Ship::Ship(float size)
		: _shader(Shader::Program::getProgram("data/shader/ship.glsl"))
		, _size(size)
		, _rotation(0.f)
		, _moving(false)
		, _rotating(0)
		, _reloadTime(0.f)
		, _invicibility(0.f)
	{
		_physicsComponent.setTerminalVelocity(225.f);

		std::vector<glm::vec2> vertices = {
			glm::vec2(0.25f * _size, 0.90f * _size), // 0
			glm::vec2(0.50f * _size, 0.10f * _size), // 1
			glm::vec2(0.75f * _size, 0.90f * _size), // 2
			glm::vec2(0.70f * _size, 0.75f * _size), // 3
			glm::vec2(0.60f * _size, 0.70f * _size), // 4
			glm::vec2(0.40f * _size, 0.70f * _size), // 5
			glm::vec2(0.30f * _size, 0.75f * _size), // 6
			glm::vec2(0.50f * _size, 0.99f * _size)  // 7
		};

		std::vector<GLuint> elements = {
			0, 1, 2, 3, 4, 5, 6, 3, 7, 6
		};

		_mesh.reset(new Geometry::Mesh(vertices, elements));
		_rotatedMesh = std::unique_ptr<Geometry::Mesh>(new Geometry::Mesh(*_mesh.get()));

		for (int i = 0; i < 5; i++)
			_projectiles.push_back(std::make_shared<Projectile>());

		reset();
	}

	void Ship::reset()
	{
		_physicsComponent.reset(glm::vec2(400.0f - _size / 2.f, 300 - _size / 2.f), glm::vec2(0.f));
		_physicsComponent.setAcceleration(glm::vec2(0.f, 0.f));
		_moving = false;
		_invicibility = 0.f;
		//_rotation = 0.f;

		for (auto p : _projectiles)
		{
			p->reload();
		}
	}

	void Ship::makeInvincible()
	{
		_invicibility = 2.5f;
	}

	bool Ship::isInvincible()
	{
		return _invicibility > 0.f;
	}

	void Ship::accelerate()
	{
		_moving = true;
	}

	void Ship::rotate(int rotation)
	{
		_rotating = rotation;
	}

	const std::shared_ptr<Projectile> Ship::shoot()
	{
		if (_reloadTime > 0.f)
		{
			return nullptr;
		}

		std::shared_ptr<Projectile> projectile = nullptr;
		for (auto p : _projectiles)
		{
			if (p->isLaunched() == false)
			{
				projectile = p;
				break;
			}
		}

		if (projectile)
		{
			glm::vec2 dir = glm::rotate(glm::vec2(0.f, -1), _rotation);
			glm::vec2 position = _physicsComponent.getPosition() + glm::vec2(_size / 2.f) + (dir * (float)_size / 2.f);
			projectile->shoot(position, dir);
			_reloadTime = 0.20f;
		}

		return projectile;
	}

	void Ship::update(float delta)
	{
		if (_moving)
		{
			float acceleration = 175.f;
			_physicsComponent.setAcceleration(glm::rotate(glm::vec2(0.f, -acceleration), _rotation));
		}
		else
		{
			_physicsComponent.setAcceleration(glm::vec2(0.f, 0.f));
		}

		_physicsComponent.update(delta);


		if (_reloadTime > 0.f)
		{
			_reloadTime -= delta;
		}

		if (_invicibility > 0.f)
		{
			_invicibility -= delta;
		}

		if (_rotating != 0)
		{
			_rotation += 4.5f * (float)_rotating * delta;
			_rotatedMesh = std::move(_mesh->rotate(_rotation, glm::vec2(_size / 2.0f)));
			_rotating = 0;
		}

		_colliderComponent.setCollisionMesh(_rotatedMesh.get());

		_colliderComponent.setPosition(_physicsComponent.getPosition());
	}

	void Ship::draw()
	{
		_shader->use();

		if (!(isInvincible() && std::fmod(glfwGetTime(), 0.1) >= 0.05))
		for (int y = -1; y <= 1; ++y)
		{
			for (int x = -1; x <= 1; ++x)
			{
				_shader->uniform("position") = _physicsComponent.getPosition() + glm::vec2(800 * x, 600 * y);
				_rotatedMesh->draw(GL_LINE_STRIP, 7, 0);
				if (glm::length(_physicsComponent.getAcceleration()) > 0.f && std::fmod(glfwGetTime(), 0.2) >= 0.1)
				{
					_rotatedMesh->draw(GL_LINE_STRIP, 3, 7);
				}
			}
		}

		_moving = false;
	}

	const Components::ColliderComponent* Ship::getColliderComponent()
	{
		return &_colliderComponent;
	}

	const Components::PhysicsComponent* Ship::getPhysicsComponent()
	{
		return &_physicsComponent;
	}
}
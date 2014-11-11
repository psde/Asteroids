#include <random>
#include <iostream>

#include <glm/gtx/rotate_vector.hpp>

#include "UFO.h"

#include <GLFW/glfw3.h>

namespace Game
{
	UFO::UFO(float size)
		: _shader(Shader::Program::getProgram("data/shader/ship.glsl"))
		, _size(size)
		, _reloadTime(0.f)
	{
		_physicsComponent.setTerminalVelocity(225.f);

		_physicsComponent.reset(glm::vec2(200, 200), glm::vec2(10.f));
		_movementTimeRemaining = 0.f;

		std::vector<glm::vec2> vertices = {
			glm::vec2(0.10f * _size, 0.50f * _size),
			glm::vec2(0.30f * _size, 0.65f * _size),
			glm::vec2(0.70f * _size, 0.65f * _size),
			glm::vec2(0.90f * _size, 0.50f * _size),
			glm::vec2(0.10f * _size, 0.50f * _size),
			glm::vec2(0.30f * _size, 0.35f * _size),
			glm::vec2(0.40f * _size, 0.35f * _size),
			glm::vec2(0.42f * _size, 0.15f * _size),
			glm::vec2(0.58f * _size, 0.15f * _size),
			glm::vec2(0.60f * _size, 0.35f * _size),
			glm::vec2(0.30f * _size, 0.35f * _size),
			glm::vec2(0.70f * _size, 0.35f * _size),
			glm::vec2(0.90f * _size, 0.50f * _size)
		};

		_mesh.reset(new Geometry::Mesh(vertices));
		_colliderComponent.setCollisionMesh(_mesh.get());

		for (int i = 0; i < 1; i++)
			_projectiles.push_back(std::make_shared<Projectile>(1.5f, false));
	}

	const std::shared_ptr<Projectile> UFO::shoot()
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
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> rotation(-1.75f, 1.75f);

			glm::vec2 norm = glm::normalize(_physicsComponent.getVelocity());
			glm::vec2 dir = glm::rotate(glm::vec2(1.f, 0.f), std::atan2(norm.x, norm.y) + (float)rotation(gen));
			glm::vec2 position = _physicsComponent.getPosition() + glm::vec2(_size / 2.f) + (dir * (float)_size / 2.f);
			projectile->shoot(position, dir);
			_reloadTime = 2.f;
		}

		return projectile;
	}

	void UFO::update(float delta)
	{
		_physicsComponent.update(delta);

		_movementTimeRemaining -= delta;
		if(_movementTimeRemaining <= 0.f)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> rotation(-0.25f, 0.25f);

			glm::vec2 norm = glm::normalize(_physicsComponent.getVelocity());
			glm::vec2 direction = glm::rotate(glm::vec2(1.f, 0.f), std::atan2(norm.x, norm.y) + (float)rotation(gen));

			_physicsComponent.reset(_physicsComponent.getPosition(), direction * 150.f);
			_movementTimeRemaining = 2.f;
		}

		if (_reloadTime > 0.f)
		{
			_reloadTime -= delta;
		}

		_colliderComponent.setPosition(_physicsComponent.getPosition());
	}

	void UFO::draw()
	{
		_shader->use();

		for (int y = -1; y <= 1; ++y)
		{
			for (int x = -1; x <= 1; ++x)
			{
				_shader->uniform("position") = _physicsComponent.getPosition() + glm::vec2(800 * x, 600 * y);
				_mesh->draw(Geometry::Mesh::LINE_STRIP);
			}
		}
	}

	const Components::ColliderComponent* UFO::getColliderComponent()
	{
		return &_colliderComponent;
	}

	const Components::PhysicsComponent* UFO::getPhysicsComponent()
	{
		return &_physicsComponent;
	}
}
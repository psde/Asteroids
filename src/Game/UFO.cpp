#include <random>
#include <iostream>

#include "Math/Math.h"

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

		_physicsComponent.reset(Math::vec2(200, 200), Math::vec2(10.f));
		_movementTimeRemaining = 0.f;

		//       6--7
		//       |  |
		//   4---8--9---5
		//  /            \
		// 0--------------1
		//  \            /
		//   2----------3

		std::vector<Math::vec2> vertices = {
			Math::vec2(0.10f * _size, 0.50f * _size), // 0
			Math::vec2(0.90f * _size, 0.50f * _size), // 1
			Math::vec2(0.30f * _size, 0.65f * _size), // 2
			Math::vec2(0.70f * _size, 0.65f * _size), // 3
			Math::vec2(0.30f * _size, 0.35f * _size), // 4
			Math::vec2(0.70f * _size, 0.35f * _size), // 5
			Math::vec2(0.42f * _size, 0.15f * _size), // 6
			Math::vec2(0.58f * _size, 0.15f * _size), // 7
			Math::vec2(0.40f * _size, 0.35f * _size), // 8
			Math::vec2(0.60f * _size, 0.35f * _size)  // 9
		};

		auto R = Geometry::Mesh::PrimitiveRestartIndex;
		std::vector<GLuint> elements = {
			// Bottom Part
			0, 2, 3, 1, R,

			// Middle Part
			0, 1, R,

			// Top Part
			0, 4, 5, 1, R,

			// Cockpit
			8, 6, 7, 9
		};

		_mesh.reset(new Geometry::Mesh(vertices, elements));
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
			std::uniform_real_distribution<float> rotation(-1.75f, 1.75f);

			Math::vec2 norm = Math::normalize(_physicsComponent.getVelocity());
			Math::vec2 dir = Math::rotate(Math::vec2(1.f, 0.f), std::atan2(norm.x, norm.y) + (float)rotation(gen));
			Math::vec2 position = _physicsComponent.getPosition() + Math::vec2(_size / 2.f) + (dir * (float)_size / 2.f);
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
			std::uniform_real_distribution<float> rotation(-0.25f, 0.25f);

			Math::vec2 norm = Math::normalize(_physicsComponent.getVelocity());
			Math::vec2 direction = Math::rotate(Math::vec2(1.f, 0.f), std::atan2(norm.x, norm.y) + (float)rotation(gen));

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
				_shader->uniform("position") = _physicsComponent.getPosition() + Math::vec2(800 * x, 600 * y);
				_mesh->draw(Geometry::Mesh::LINE_STRIP);
			}
		}
	}

	const Components::ColliderComponent* UFO::getColliderComponent() const
	{
		return &_colliderComponent;
	}

	const Components::PhysicsComponent* UFO::getPhysicsComponent() const
	{
		return &_physicsComponent;
	}
}
#include <random>
#include <iostream>

#include "Math/Math.h"
#include "Graphics/Graphics.h"

#include "Ship.h"

namespace Game
{
	Ship::Ship(float size)
		: GameObject(new Components::CollisionComponent(), new Components::PhysicsRungeKutta())
		, _shader(Graphics::Program::getProgram("data/shader/entity.glsl"))
		, _size(size)
		, _rotation(0.f)
		, _moving(false)
		, _rotating(0)
		, _reloadTime(0.f)
		, _invicibility(0.f)
		, _score(0)
	{
		resetLives();

		_physicsComponent->setTerminalVelocity(225.f);

		std::vector<Math::vec2> vertices = {
			Math::vec2(0.25f * _size, 0.90f * _size), // 0
			Math::vec2(0.50f * _size, 0.10f * _size), // 1
			Math::vec2(0.75f * _size, 0.90f * _size), // 2
			Math::vec2(0.70f * _size, 0.75f * _size), // 3
			Math::vec2(0.60f * _size, 0.70f * _size), // 4
			Math::vec2(0.40f * _size, 0.70f * _size), // 5
			Math::vec2(0.30f * _size, 0.75f * _size), // 6
			Math::vec2(0.50f * _size, 0.99f * _size)  // 7
		};

		auto R = Graphics::GeometryRestartIndex;
		std::vector<GLuint> elements = {
			0, 1, 2, R, 3, 4, 5, 6,
			
			// Exhaust Flames
			3, 7, 6
		};

		_mesh.reset(new Graphics::Mesh(vertices, elements));
		_rotatedMesh = std::unique_ptr<Graphics::Mesh>(new Graphics::Mesh(*_mesh.get()));

		for (int i = 0; i < 5; i++)
			_projectiles.push_back(std::make_shared<Projectile>(2.f, true));

		resetPosition();
	}

	void Ship::resetPosition()
	{
		_physicsComponent->reset(Math::vec2(400.0f - _size / 2.f, 300 - _size / 2.f), Math::vec2(0.f));
		_physicsComponent->setAcceleration(Math::vec2(0.f, 0.f));
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
			Math::vec2 dir = Math::rotate(Math::vec2(0.f, -1.f), _rotation);
			Math::vec2 position = _physicsComponent->getPosition() + Math::vec2(_size / 2.f) + (dir * (float)_size / 2.f);
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
			_physicsComponent->setAcceleration(Math::rotate(Math::vec2(0.f, -acceleration), _rotation));
		}
		else
		{
			_physicsComponent->setAcceleration(Math::vec2(0.f, 0.f));
		}

		_physicsComponent->update(delta);


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
			_rotatedMesh = std::move(_mesh->rotate(_rotation, Math::vec2(_size / 2.0f)));
			_rotating = 0;
		}

		_collisionComponent->setCollisionMesh(_rotatedMesh.get());

		_collisionComponent->setPosition(_physicsComponent->getPosition());
	}

	void Ship::draw()
	{
		_shader->use();
		_shader->uniform("size") = 1.0f;

		if (!(isInvincible() && std::fmod(Graphics::getTime(), 0.1) >= 0.05))
		for (int y = -1; y <= 1; ++y)
		{
			for (int x = -1; x <= 1; ++x)
			{
				_shader->uniform("position") = _physicsComponent->getPosition() + Math::vec2(800 * x, 600 * y);
				_rotatedMesh->draw(Graphics::LINE_STRIP, 8, 0);
				if (Math::length(_physicsComponent->getAcceleration()) > 0.f && std::fmod(Graphics::getTime(), 0.2) >= 0.1)
				{
					_rotatedMesh->draw(Graphics::LINE_STRIP, 3, 8);
				}
			}
		}

		_moving = false;
	}

	void Ship::addScore(unsigned int points)
	{
		if (_score + points >= 10000)
		{
			_score -= 10000;
			_lives += 3;
		}
		else
		{
			for (unsigned int i = 1000; i < 9000; i+=1000)
			{
				if (_score < i && _score + points >= i)
				{
					_lives++;
				}
			}
		}

		if (_lives > 6)
			_lives = 6;

		_score += points;
	}
}
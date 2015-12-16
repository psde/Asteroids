#include <random>
#include <iostream>

#include "Math/Math.h"
#include "Graphics/Window/Window.h"

#include "ParticleEmitter.h"
#include "Ship.h"

namespace Game
{
	Ship::Ship(float size)
		: GameObject(new Components::CollisionComponent(), new Components::PhysicsRungeKutta())
		, m_shader(Graphics::Program::getProgram("data/shader/entity.glsl"))
		, m_size(size)
		, m_rotation(0.f)
		, m_moving(false)
		, m_rotating(0)
		, m_reloadTime(0.f)
		, m_invicibility(0.f)
		, m_score(0)
	{
		resetLives();

		m_physicsComponent->setTerminalVelocity(225.f);

		std::vector<Math::vec2> vertices = {
			Math::vec2(0.25f * m_size, 0.90f * m_size), // 0
			Math::vec2(0.50f * m_size, 0.10f * m_size), // 1
			Math::vec2(0.75f * m_size, 0.90f * m_size), // 2
			Math::vec2(0.70f * m_size, 0.75f * m_size), // 3
			Math::vec2(0.60f * m_size, 0.70f * m_size), // 4
			Math::vec2(0.40f * m_size, 0.70f * m_size), // 5
			Math::vec2(0.30f * m_size, 0.75f * m_size), // 6
			Math::vec2(0.50f * m_size, 0.99f * m_size)  // 7
		};

		auto R = Graphics::GeometryRestartIndex;
		std::vector<GLuint> elements = {
			0, 1, 2, R, 3, 4, 5, 6,
			
			// Exhaust Flames
			3, 7, 6
		};

		m_mesh.reset(new Graphics::Mesh(vertices, elements));
		m_rotatedMesh = std::unique_ptr<Graphics::Mesh>(new Graphics::Mesh(*m_mesh.get()));

		for (int i = 0; i < 5; i++)
			m_projectiles.push_back(std::make_shared<Projectile>(2.f, true));

		resetPosition();
	}

	void Ship::resetPosition()
	{
		m_physicsComponent->reset(Math::vec2(400.0f - m_size / 2.f, 300 - m_size / 2.f), Math::vec2(0.f));
		m_physicsComponent->setAcceleration(Math::vec2(0.f, 0.f));
		m_moving = false;
		m_invicibility = 0.f;
		//m_rotation = 0.f;

		for (auto p : m_projectiles)
		{
			p->reload();
		}
	}

	void Ship::makeInvincible()
	{
		m_invicibility = 2.5f;
	}

	bool Ship::isInvincible()
	{
		return m_invicibility > 0.f;
	}

	const std::shared_ptr<Projectile> Ship::shoot()
	{
		if (m_reloadTime > 0.f)
		{
			return nullptr;
		}

		std::shared_ptr<Projectile> projectile = nullptr;
		for (auto p : m_projectiles)
		{
			if (p->launched() == false)
			{
				projectile = p;
				break;
			}
		}

		if (projectile)
		{
			Math::vec2 dir = Math::rotate(Math::vec2(0.f, -1.f), m_rotation);
			Math::vec2 position = m_physicsComponent->getPosition() + Math::vec2(m_size / 2.f) + (dir * (float) m_size / 2.f);
			projectile->shoot(position, dir);
			m_reloadTime = 0.20f;
		}

		return projectile;
	}

	void Ship::update(float delta)
	{
		if (m_moving)
		{
			float acceleration = 175.f;
			m_physicsComponent->setAcceleration(Math::rotate(Math::vec2(0.f, -acceleration), m_rotation));
		}
		else
		{
			m_physicsComponent->setAcceleration(Math::vec2(0.f, 0.f));
		}

		m_physicsComponent->update(delta);


		if (m_reloadTime > 0.f)
		{
			m_reloadTime -= delta;
		}

		if (m_invicibility > 0.f)
		{
			m_invicibility -= delta;
		}

		if (m_rotating != 0)
		{
			m_rotation += 4.5f * (float) m_rotating * delta;
			m_rotatedMesh = std::move(m_mesh->rotate(m_rotation, Math::vec2(m_size / 2.0f)));
			m_rotating = 0;
		}

		m_collisionComponent->setCollisionMesh(m_rotatedMesh.get());

		m_collisionComponent->setPosition(m_physicsComponent->getPosition());
	}

	void Ship::draw()
	{
		m_shader->use();
		m_shader->uniform("size") = 1.0f;

		auto time = Graphics::Window::instance().time();
		if (!(isInvincible() && std::fmod(time, 0.1) >= 0.05))
		for (int y = -1; y <= 1; ++y)
		{
			for (int x = -1; x <= 1; ++x)
			{
				m_shader->uniform("position") = m_physicsComponent->getPosition() + Math::vec2(800 * x, 600 * y);
				m_rotatedMesh->draw(Graphics::DrawMode::LineStrip, 8, 0);
				if (Math::length(m_physicsComponent->getAcceleration()) > 0.f && std::fmod(time, 0.2) >= 0.1)
				{
					m_rotatedMesh->draw(Graphics::DrawMode::LineStrip, 3, 8);
				}
			}
		}

		m_moving = false;
	}

	void Ship::addScore(unsigned int points)
	{
		if (m_score + points >= 10000)
		{
			m_score -= 10000;
			m_lives += 3;
		}
		else
		{
			for (unsigned int i = 1000; i < 9000; i+=1000)
			{
				if (m_score < i && m_score + points >= i)
				{
					m_lives++;
				}
			}
		}

		if (m_lives > 6)
			m_lives = 6;

		m_score += points;
	}

	const std::shared_ptr<Projectile> Ship::handleInput()
	{
		auto& window = Graphics::Window::instance();
		if (window.getKeyState(Graphics::KEY_UP) == Graphics::KeyState::Press)
		{
			m_moving = true;
		}

		if (window.getKeyState(Graphics::KEY_LEFT) == Graphics::KeyState::Press)
		{
			m_rotating = -1;
		}
		else if (window.getKeyState(Graphics::KEY_RIGHT) == Graphics::KeyState::Press)
		{
			m_rotating = 1;
		}

		if (window.getKeyState(Graphics::KEY_SPACE) == Graphics::KeyState::Press)
		{
			return shoot();
		}
		return nullptr;
	}

	void Ship::destroy()
	{
		ParticleEmitter::instance().emitParticles(physicsComponent()->getPosition() + 12.5f, 5, 5);
	}
}

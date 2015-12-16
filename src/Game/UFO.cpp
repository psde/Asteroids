#include <random>
#include <iostream>

#include "Math/Math.h"

#include "ParticleEmitter.h"
#include "UFO.h"

namespace Game
{
	UFO::UFO(float size)
		: GameObject(new Components::CollisionComponent(), new Components::PhysicsRungeKutta())
		, m_shader(Graphics::Program::getProgram("data/shader/entity.glsl"))
		, m_size(size)
		, m_reloadTime(0.f)
	{
		m_physicsComponent->setTerminalVelocity(225.f);

		m_physicsComponent->reset(Math::vec2(200, 200), Math::vec2(10.f));
		m_movementTimeRemaining = 0.f;

		/*
		       6--7
		       |  |
		   4---8--9---5
		  /            \
		 0--------------1
		  \            /
		   2----------3
		*/

		std::vector<Math::vec2> vertices = {
			Math::vec2(0.10f * m_size, 0.50f * m_size), // 0
			Math::vec2(0.90f * m_size, 0.50f * m_size), // 1
			Math::vec2(0.30f * m_size, 0.65f * m_size), // 2
			Math::vec2(0.70f * m_size, 0.65f * m_size), // 3
			Math::vec2(0.30f * m_size, 0.35f * m_size), // 4
			Math::vec2(0.70f * m_size, 0.35f * m_size), // 5
			Math::vec2(0.42f * m_size, 0.15f * m_size), // 6
			Math::vec2(0.58f * m_size, 0.15f * m_size), // 7
			Math::vec2(0.40f * m_size, 0.35f * m_size), // 8
			Math::vec2(0.60f * m_size, 0.35f * m_size)  // 9
		};

		auto R = Graphics::GeometryRestartIndex;
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

		m_mesh.reset(new Graphics::Mesh(vertices, elements));
		m_collisionComponent->setCollisionMesh(m_mesh.get());

		for (int i = 0; i < 1; i++)
			m_projectiles.push_back(std::make_shared<Projectile>(1.5f, false));
	}

	const std::shared_ptr<Projectile> UFO::shoot()
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
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> rotation(-1.75f, 1.75f);

			Math::vec2 norm = Math::normalize(m_physicsComponent->getVelocity());
			Math::vec2 dir = Math::rotate(Math::vec2(1.f, 0.f), std::atan2(norm.x, norm.y) + (float)rotation(gen));
			Math::vec2 position = m_physicsComponent->getPosition() + Math::vec2(m_size / 2.f) + (dir * (float) m_size / 2.f);
			projectile->shoot(position, dir);
			m_reloadTime = 2.f;
		}

		return projectile;
	}

	void UFO::update(float delta)
	{
		m_physicsComponent->update(delta);

		m_movementTimeRemaining -= delta;
		if(m_movementTimeRemaining <= 0.f)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<float> rotation(-0.25f, 0.25f);

			Math::vec2 norm = Math::normalize(m_physicsComponent->getVelocity());
			Math::vec2 direction = Math::rotate(Math::vec2(1.f, 0.f), std::atan2(norm.x, norm.y) + (float)rotation(gen));

			m_physicsComponent->reset(m_physicsComponent->getPosition(), direction * 150.f);
			m_movementTimeRemaining = 2.f;
		}

		if (m_reloadTime > 0.f)
		{
			m_reloadTime -= delta;
		}

		m_collisionComponent->setPosition(m_physicsComponent->getPosition());
	}

	void UFO::draw()
	{
		m_shader->use();
		m_shader->uniform("size") = 1.0f;

		for (int y = -1; y <= 1; ++y)
		{
			for (int x = -1; x <= 1; ++x)
			{
				m_shader->uniform("position") = m_physicsComponent->getPosition() + Math::vec2(800 * x, 600 * y);
				m_mesh->draw(Graphics::DrawMode::LineStrip);
			}
		}
	}

	void UFO::destroy()
	{
		ParticleEmitter::instance().emitParticles(physicsComponent()->getPosition() + 20.f, 10, 25 );
	}
}

#include <random>
#include <iostream>

#include "Math/Math.h"

#include "Projectile.h"

namespace Game
{
	Projectile::Projectile(float lifetime, bool friendly)
		: GameObject(new Components::CollisionComponent(), new Components::PhysicsEuler())
		, m_shader(Graphics::Program::getProgram("data/shader/entity.glsl"))
		, m_launched(false)
		, m_time(0.f)
		, m_lifetime(lifetime)
		, m_friendly(friendly)
	{
		m_physicsComponent->reset(Math::vec2(0.f), Math::vec2(0.f));

		float size = 1.15f;

		int steps = 6;
		float r = 0.f;
		std::vector<Math::vec2> vertices;
		for (int i = 0; i <= steps; i++)
		{
			r += ((2.f * Math::pi<float>()) / (float)steps);
			vertices.push_back(Math::vec2(sin(r), cos(r)) * size);
		}

		m_mesh.reset(new Graphics::Mesh(vertices));
		m_collisionComponent->setCollisionMesh(m_mesh.get());
	}

	void Projectile::update(float delta)
	{
		if(launched() == false)
			return;

		m_time -= delta;
		m_physicsComponent->update(delta);

		m_collisionComponent->setPosition(m_physicsComponent->getPosition());

		if (m_time <= 0.f)
		{
			m_launched = false;
		}
	}

	void Projectile::draw()
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

	bool Projectile::launched()
	{
		return m_launched;
	}

	void Projectile::shoot(Math::vec2 position, Math::vec2 direction)
	{
		m_launched = true;
		m_time = m_lifetime;
		m_physicsComponent->reset(position, direction * 275.f);
	}

	void Projectile::reload()
	{
		m_launched = false;
		m_physicsComponent->reset(Math::vec2(0.f), Math::vec2(0.f));
	}
}

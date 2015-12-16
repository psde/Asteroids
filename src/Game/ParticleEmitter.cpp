#include <random>

#include "ParticleEmitter.h"

namespace Game
{
	Particle::Particle(std::shared_ptr<Graphics::Mesh> mesh, Math::vec2 position, Math::vec2 direction)
		: GameObject(nullptr, new Components::PhysicsEuler())
		, m_shader(Graphics::Program::getProgram("data/shader/particle.glsl"))
		, m_mesh(mesh)
	{
		m_physicsComponent->reset(position, direction * 10.f);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> size(0.25f, 0.75f);
		std::uniform_real_distribution<float> time(2.2f, 3.75f);

		m_remainingTime = time(gen);
		m_particleSize = size(gen);
	}

	void Particle::update(float timeDelta)
	{
		m_physicsComponent->update(timeDelta);
		m_physicsComponent->setAcceleration(Math::vec2(-1.f));
		m_remainingTime -= timeDelta;
	}

	void Particle::draw()
	{
		m_shader->use();

		m_shader->uniform("remainingTime") = m_remainingTime;
		m_shader->uniform("size") = m_particleSize;
		m_shader->uniform("position") = m_physicsComponent->getPosition();
		m_mesh->draw(Graphics::DrawMode::LineStrip);
	}

	ParticleEmitter::ParticleEmitter()
		: GameObject(nullptr, nullptr)
	{
		// Preload shader so it does not happen mid-game
		auto shader = Graphics::Program::getProgram("data/shader/particle.glsl");

		int steps = 12;
		float r = 0.f;
		std::vector<Math::vec2> vertices;
		for (int i = 0; i <= steps; i++)
		{
			r += ((2.f * Math::pi<float>()) / (float)steps);
			vertices.push_back(Math::vec2(sin(r), cos(r)));
		}

		m_mesh.reset(new Graphics::Mesh(vertices));
	}

	ParticleEmitter &ParticleEmitter::instance()
	{
		static ParticleEmitter instance;
		return instance;
	}

	void ParticleEmitter::emitParticles(Math::vec2 position, float radius, int count)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> offset(-radius, radius);
		std::uniform_real_distribution<float> foo(0.f, 1.f);

		for (int i = 0; i < count; i++)
		{
			Math::vec2 pos = position + Math::vec2(offset(gen), offset(gen));

			float t = 2.f * Math::pi<float>() * foo(gen);
			pos = position + Math::vec2(offset(gen) * cos(t), offset(gen) * sin(t));

			Math::vec2 dir = Math::normalize(pos - position);
			m_particles.push_back(std::unique_ptr<Particle>(new Particle(m_mesh, pos, dir)));
		}
	}

	void ParticleEmitter::reset()
	{
		m_particles.clear();
	}

	void ParticleEmitter::update(float timeDelta)
	{
		auto particleIterator = std::begin(m_particles);
		while (particleIterator != std::end(m_particles)) {
			auto &particle = *particleIterator;
			particle->update(timeDelta);

			if (particle->isDead())
			{
				particleIterator = m_particles.erase(particleIterator);
			}
			else
			{
				particleIterator++;
			}
		}
	}

	void ParticleEmitter::draw()
	{
		for (auto &p : m_particles)
		{
			p->draw();
		}
	}

}

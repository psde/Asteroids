#include <random>

#include "ParticleEmitter.h"

namespace Game
{
	Particle::Particle(std::shared_ptr<Graphics::Mesh> mesh, Math::vec2 position, Math::vec2 direction)
		: _shader(Graphics::Program::getProgram("data/shader/particle.glsl"))
		, _mesh(mesh)
	{
		_physicsComponent.reset(position, direction * 10.f);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> size(0.25f, 0.75f);
		std::uniform_real_distribution<float> time(2.2f, 3.75f);

		_remainingTime = time(gen);
		_particleSize = size(gen);
	}

	void Particle::update(float timeDelta)
	{
		_physicsComponent.update(timeDelta);
		_physicsComponent.setAcceleration(Math::vec2(-1.f));
		_remainingTime -= timeDelta;
	}

	void Particle::draw()
	{
		_shader->use();

		_shader->uniform("remainingTime") = _remainingTime;
		_shader->uniform("particleSize") = _particleSize;
		_shader->uniform("position") = _physicsComponent.getPosition();
		_mesh->draw(Graphics::LINE_STRIP);
	}

	ParticleEmitter::ParticleEmitter()
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

		_mesh.reset(new Graphics::Mesh(vertices));
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
			_particles.push_back(new Particle(_mesh, pos, dir));
		}
	}

	void ParticleEmitter::reset()
	{
		for(auto p : _particles)
		{
			delete p;
		}
		_particles.clear();
	}

	void ParticleEmitter::update(float timeDelta)
	{
		auto particleIterator = std::begin(_particles);
		while (particleIterator != std::end(_particles)) {
			auto particle = *particleIterator;
			particle->update(timeDelta);

			if (particle->isDead())
			{
				delete particle;
				particleIterator = _particles.erase(particleIterator);
			}
			else
			{
				particleIterator++;
			}
		}
	}

	void ParticleEmitter::draw()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		for (auto p : _particles)
		{
			p->draw();
		}
		glDisable(GL_BLEND);
	}
}

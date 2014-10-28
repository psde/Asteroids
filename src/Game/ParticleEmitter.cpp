#include <random>

#include "ParticleEmitter.h"

namespace Game
{
	Particle::Particle(glm::vec2 position, glm::vec2 direction)
		: _shader(Shader::Program::getProgram("data/shader/particle.glsl"))
	{
		_physicsComponent.reset(position, direction * 10.f);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> offset(0.f, 0.5f);

		_remainingTime = 2.f + offset(gen);
		float size = 0.25f + offset(gen);

		int steps = 6;
		float r = 0.f;
		std::vector<glm::vec2> vertices;
		for (int i = 0; i <= steps; i++)
		{
			r += ((2.f * glm::pi<float>()) / (float)steps);
			vertices.push_back(glm::vec2(sin(r), cos(r)) * size);
		}

		_mesh.reset(new Geometry::Mesh(vertices));
	}

	void Particle::update(float timeDelta)
	{
		_physicsComponent.update(timeDelta);
		_physicsComponent.setAcceleration(glm::vec2(-1.f));
		_remainingTime -= timeDelta;
	}

	void Particle::draw()
	{
		_shader->use();

		_shader->uniform("remainingTime") = _remainingTime;
		for (int y = -1; y < 1; ++y)
		{
			for (int x = -1; x < 1; ++x)
			{
				_shader->uniform("position") = _physicsComponent.getPosition() + glm::vec2(800 * x, 600 * y);
				_mesh->draw(GL_LINE_STRIP);
			}
		}
	}


	ParticleEmitter::ParticleEmitter()
	{

	}

	void ParticleEmitter::emitParticles(glm::vec2 position, float radius, int count)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> offset(-radius, radius);
		std::uniform_real_distribution<> foo(0.f, 1.f);

		for (int i = 0; i < count; i++)
		{
			glm::vec2 pos = position + glm::vec2(offset(gen), offset(gen));

			float t = 2.f * glm::pi<float>() * foo(gen);
			pos = position + glm::vec2(offset(gen) * cos(t), offset(gen) * sin(t));

			glm::vec2 dir = glm::normalize(pos - position);
			_particles.push_back(new Particle(pos, dir));
		}
	}

	void ParticleEmitter::update(float timeDelta)
	{
		auto particleIterator = std::begin(_particles);
		while (particleIterator != std::end(_particles)) {
			auto particle = *particleIterator;
			particle->update(timeDelta);

			if (particle->isDead())
			{
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

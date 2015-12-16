#pragma once

#include <vector>
#include <memory>

#include "Math/Math.h"

#include "Graphics/Geometry/Mesh.h"

#include "Components/PhysicsEuler.h"

#include "GameObject.h"

namespace Game
{
	class Particle : public GameObject
	{
	private:
		float _remainingTime;
		float _particleSize;
		std::shared_ptr<Graphics::Program> _shader;
		std::shared_ptr<Graphics::Mesh> _mesh;

	public:
		Particle(std::shared_ptr<Graphics::Mesh> mesh, Math::vec2 position, Math::vec2 direction);

		void update(float timeDelta);
		void draw();

		bool isDead() { return _remainingTime <= 0.f; }
	};

	class ParticleEmitter : public GameObject
	{
	private:
		std::vector<std::unique_ptr<Particle>> _particles;
		std::shared_ptr<Graphics::Mesh> _mesh;

		ParticleEmitter();
	public:
		static ParticleEmitter& instance();
		ParticleEmitter(const ParticleEmitter&) = delete;
		ParticleEmitter(ParticleEmitter&&) = delete;
		ParticleEmitter operator=(const ParticleEmitter&) = delete;
		ParticleEmitter operator=(ParticleEmitter&&) = delete;

		void emitParticles(Math::vec2 position, float radius, int count);
		void reset();

		void update(float timeDelta) override;
		void draw() override;
	};
}

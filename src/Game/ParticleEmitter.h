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
		Components::PhysicsEuler _physicsComponent;
		float _remainingTime;
		float _particleSize;
		std::shared_ptr<Graphics::Program> _shader;
		std::shared_ptr<Graphics::Mesh> _mesh;

	public:
		Particle(std::shared_ptr<Graphics::Mesh> mesh, Math::vec2 position, Math::vec2 direction);

		void update(float timeDelta);
		void draw();
		const Components::CollisionComponent* getCollisionComponent() const { return nullptr; }
		const Components::PhysicsComponent* getPhysicsComponent() const { return &_physicsComponent; }

		bool isDead() { return _remainingTime <= 0.f; }
	};

	class ParticleEmitter : public GameObject
	{
	private:
		std::vector<Particle*> _particles;
		std::shared_ptr<Graphics::Mesh> _mesh;

	public:
		ParticleEmitter();

		void emitParticles(Math::vec2 position, float radius, int count);
		void reset();

		void update(float timeDelta);
		void draw();
		const Components::CollisionComponent* getCollisionComponent() const { return nullptr; }
		const Components::PhysicsComponent* getPhysicsComponent() const { return nullptr; }
	};
}

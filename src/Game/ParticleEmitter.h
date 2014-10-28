#pragma once

#include <vector>
#include <memory>

#include <glm/glm.hpp>

#include "Geometry/Mesh.h"

#include "PhysicsComponent.h"
#include "GameObject.h"

namespace Game
{
	class Particle : public GameObject
	{
	private:
		PhysicsComponent _physicsComponent;
		float _remainingTime;
		std::unique_ptr<Geometry::Mesh> _mesh;
		std::shared_ptr<Shader::Program> _shader;

	public:
		Particle(glm::vec2 position, glm::vec2 direction);

		void update(float timeDelta);
		void draw();
		const ColliderComponent* getColliderComponent() { return nullptr; }
		const PhysicsComponent* getPhysicsComponent() { return &_physicsComponent; }

		bool isDead() { return _remainingTime <= 0.f; }
	};

	class ParticleEmitter : public GameObject
	{
	private:
		std::vector<Particle*> _particles;

	public:
		ParticleEmitter();

		void emitParticles(glm::vec2 position, float radius, int count);

		void update(float timeDelta);
		void draw();
		const ColliderComponent* getColliderComponent() { return nullptr; }
		const PhysicsComponent* getPhysicsComponent() { return nullptr; }
	};
}

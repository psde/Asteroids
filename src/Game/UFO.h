#pragma once

#include <memory>

#include "Math/Math.h"

#include "Geometry/Mesh.h"

#include "GameObject.h"
#include "Components/PhysicsRungeKutta.h"
#include "Components/ColliderComponent.h"
#include "Projectile.h"

namespace Game
{
	// Implements the UFO game object, which is controlled by an AI
	class UFO : public GameObject
	{
	private:
		Components::ColliderComponent _colliderComponent;
		Components::PhysicsRungeKutta _physicsComponent;

		std::unique_ptr<Geometry::Mesh> _mesh;
		std::shared_ptr<Shader::Program> _shader;

		int _size;
		float _reloadTime;

		std::vector<std::shared_ptr<Projectile>> _projectiles;

		float _movementTimeRemaining;

	public:
		UFO(float size = 50.f);

		const std::shared_ptr<Projectile> shoot();

		// Advances the ships simulation by the given time step
		void update(float delta);

		// Draws the UFO at its current position
		void draw();

		// See GameObject
		const Components::ColliderComponent* getColliderComponent() const;

		// See GameObject
		const Components::PhysicsComponent* getPhysicsComponent() const;
	};
}
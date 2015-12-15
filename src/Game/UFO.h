#pragma once

#include <memory>

#include "Math/Math.h"

#include "Graphics/Geometry/Mesh.h"

#include "GameObject.h"
#include "Components/PhysicsRungeKutta.h"
#include "Components/CollisionComponent.h"
#include "Projectile.h"

namespace Game
{
	// Implements the UFO game object, which is controlled by an AI
	class UFO : public GameObject
	{
	private:
		std::unique_ptr<Graphics::Mesh> _mesh;
		std::shared_ptr<Graphics::Program> _shader;

		float _size;
		float _reloadTime;

		std::vector<std::shared_ptr<Projectile>> _projectiles;

		float _movementTimeRemaining;

	public:
		UFO(float size = 50.f);

		const std::shared_ptr<Projectile> shoot();

		// Advances the ships simulation by the given time step
		void update(float delta) override;

		// Draws the UFO at its current position
		void draw() override;
	};
}
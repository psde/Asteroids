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
		std::unique_ptr<Graphics::Mesh> m_mesh;
		std::shared_ptr<Graphics::Program> m_shader;

		float m_size;
		float m_reloadTime;

		std::vector<std::shared_ptr<Projectile>> m_projectiles;

		float m_movementTimeRemaining;

	public:
		UFO(float size = 50.f);

		const std::shared_ptr<Projectile> shoot();

		// Advances the ships simulation by the given time step
		void update(float delta) override;

		void destroy() override;

		// Draws the UFO at its current position
		void draw() override;
	};
}

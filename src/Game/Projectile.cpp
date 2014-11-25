#include <random>
#include <iostream>

//#include <glm/gtx/rotate_vector.hpp>
//#include <glm/glm.hpp>
#include "Math/Math.h"

#include "Projectile.h"

#include <GLFW/glfw3.h>

namespace Game
{
	Projectile::Projectile(float lifetime, bool friendly)
		: _shader(Shader::Program::getProgram("data/shader/ship.glsl"))
		, _launched(false)
		, _time(0.f)
		, _lifetime(lifetime)
		, _friendly(friendly)
	{
		_physicsComponent.reset(glm::vec2(0.f), glm::vec2(0.f));

		float size = 1.15f;

		int steps = 6;
		float r = 0.f;
		std::vector<glm::vec2> vertices;
		for (int i = 0; i <= steps; i++)
		{
			r += ((2.f * glm::pi<float>()) / (float)steps);
			vertices.push_back(glm::vec2(sin(r), cos(r)) * size);
		}

		_mesh.reset(new Geometry::Mesh(vertices));
		_colliderComponent.setCollisionMesh(_mesh.get());
	}

	void Projectile::update(float delta)
	{
		if(isLaunched() == false)
			return;

		_time -= delta;
		_physicsComponent.update(delta);

		_colliderComponent.setPosition(_physicsComponent.getPosition());

		if (_time <= 0.f)
		{
			_launched = false;
		}
	}

	void Projectile::draw()
	{
		_shader->use();

		for (int y = -1; y <= 1; ++y)
		{
			for (int x = -1; x <= 1; ++x)
			{
				_shader->uniform("position") = _physicsComponent.getPosition() + glm::vec2(800 * x, 600 * y);
				_mesh->draw(Geometry::Mesh::LINE_STRIP);
			}
		}
	}

	const Components::ColliderComponent* Projectile::getColliderComponent() const
	{
		return &_colliderComponent;
	}

	const Components::PhysicsComponent* Projectile::getPhysicsComponent() const
	{
		return &_physicsComponent;
	}

	bool Projectile::isLaunched()
	{
		return _launched;
	}

	void Projectile::shoot(glm::vec2 position, glm::vec2 direction)
	{
		_launched = true;
		_time = _lifetime;
		_physicsComponent.reset(position, direction * 275.f);
	}

	void Projectile::reload()
	{
		_launched = false;
		_physicsComponent.reset(glm::vec2(0.f), glm::vec2(0.f));
	}
}
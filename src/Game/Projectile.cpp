#include <random>
#include <iostream>

#include "Math/Math.h"

#include "Projectile.h"

namespace Game
{
	Projectile::Projectile(float lifetime, bool friendly)
		: _shader(Graphics::Program::getProgram("data/shader/entity.glsl"))
		, _launched(false)
		, _time(0.f)
		, _lifetime(lifetime)
		, _friendly(friendly)
	{
		_physicsComponent.reset(Math::vec2(0.f), Math::vec2(0.f));

		float size = 1.15f;

		int steps = 6;
		float r = 0.f;
		std::vector<Math::vec2> vertices;
		for (int i = 0; i <= steps; i++)
		{
			r += ((2.f * Math::pi<float>()) / (float)steps);
			vertices.push_back(Math::vec2(sin(r), cos(r)) * size);
		}

		_mesh.reset(new Graphics::Mesh(vertices));
		_collisionComponent.setCollisionMesh(_mesh.get());
	}

	void Projectile::update(float delta)
	{
		if(isLaunched() == false)
			return;

		_time -= delta;
		_physicsComponent.update(delta);

		_collisionComponent.setPosition(_physicsComponent.getPosition());

		if (_time <= 0.f)
		{
			_launched = false;
		}
	}

	void Projectile::draw()
	{
		_shader->use();
		_shader->uniform("size") = 1.0f;

		for (int y = -1; y <= 1; ++y)
		{
			for (int x = -1; x <= 1; ++x)
			{
				_shader->uniform("position") = _physicsComponent.getPosition() + Math::vec2(800 * x, 600 * y);
				_mesh->draw(Graphics::LINE_STRIP);
			}
		}
	}

	const Components::CollisionComponent* Projectile::getCollisionComponent() const
	{
		return &_collisionComponent;
	}

	const Components::PhysicsComponent* Projectile::getPhysicsComponent() const
	{
		return &_physicsComponent;
	}

	bool Projectile::isLaunched()
	{
		return _launched;
	}

	void Projectile::shoot(Math::vec2 position, Math::vec2 direction)
	{
		_launched = true;
		_time = _lifetime;
		_physicsComponent.reset(position, direction * 275.f);
	}

	void Projectile::reload()
	{
		_launched = false;
		_physicsComponent.reset(Math::vec2(0.f), Math::vec2(0.f));
	}
}
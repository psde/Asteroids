#include <sstream>
#include <iomanip>
#include <random>
#include <iostream>

#include "Game.h"

#include <glm/gtx/rotate_vector.hpp>
#include <GLFW/glfw3.h>

namespace Game
{
	Game::Game(std::shared_ptr<Window::Window> window)
	: _window(window)
	, _livesRenderer(22.5f)
	{
		_lives = 5;
		_score = 0;
		for (int i = 0; i < 10; ++i)
		{
			_asteroids.push_back(new Asteroid(Asteroid::AsteroidSizes().size()-1));
		}
	}


	void Game::destroyAsteroid(Asteroid *asteroid)
	{
		int size = asteroid->getAsteroidSize();
		_score += (Asteroid::AsteroidSizes().size() - size) * 100;

		if (size != 0)
		{
			glm::vec2 pos = asteroid->getPhysicsComponent()->getPosition();
			glm::vec2 dir = glm::normalize(asteroid->getPhysicsComponent()->getVelocity());

			dir = glm::rotate(dir, 0.5f * glm::pi<float>());
			size--;
			_asteroids.push_back(new Asteroid(size, pos, dir));
			_asteroids.push_back(new Asteroid(size, pos, -dir));
		}

	}

	void Game::update(float timeDelta)
	{
		for (Asteroid* asteroid : _asteroids)
		{
			asteroid->update(timeDelta);
		}
		
		if (_window->getGlfwKeyState(GLFW_KEY_UP) == GLFW_PRESS)
		{
			_ship.accelerate();
		}

		int rotation = 0;
		if (_window->getGlfwKeyState(GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			rotation = -1;
		}
		else if (_window->getGlfwKeyState(GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			rotation = 1;
		}

		if (_window->getGlfwKeyState(GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			auto projectile = _ship.shoot();

			if (projectile)
				_projectiles.push_back(projectile);
		}

		//PhysicsComponent *component = const_cast<PhysicsComponent*>(_ship.getPhysicsComponent());
		//component->reset(_window->getCursorPosition(), glm::vec2(0));

		_ship.rotate(rotation);
		_ship.update(timeDelta);


		std::vector<Asteroid*> destroyedAsteroids;

		auto asteroidIterator = std::begin(_asteroids);
		while (asteroidIterator != std::end(_asteroids)) {
			bool removeAsteroid = false;

			auto projectileIterator = std::begin(_projectiles);
			while (projectileIterator != std::end(_projectiles)) {
				bool removeProjectile = !(*projectileIterator)->isLaunched();
				bool collidesProjectile = (*asteroidIterator)->getColliderComponent()->collidesWith((*projectileIterator)->getColliderComponent());

				if (collidesProjectile)
				{
					(*projectileIterator)->reload();
					removeProjectile = true;
					removeAsteroid = true;
				}

				if (removeProjectile)
				{
					projectileIterator = _projectiles.erase(projectileIterator);
				}
				else
				{

					++projectileIterator;
				}
			}

			bool collides = (*asteroidIterator)->getColliderComponent()->collidesWith(_ship.getColliderComponent());
			if (collides)
			{
				PhysicsComponent *component = const_cast<PhysicsComponent*>(_ship.getPhysicsComponent());
				component->reset(glm::vec2(400, 300), glm::vec2(0));
				_lives--;
			}

			if (removeAsteroid)
			{
				destroyedAsteroids.push_back(*asteroidIterator);
				asteroidIterator = _asteroids.erase(asteroidIterator);
			}
			else
			{
				++asteroidIterator;
			}
		}

		for (Asteroid* asteroid : destroyedAsteroids)
		{
			destroyAsteroid(asteroid);
		}
	}

	void Game::draw()
	{
		for (Asteroid* asteroid : _asteroids)
		{
			asteroid->draw();
		}

		_ship.draw();

		std::stringstream ss;
		ss << std::setw(10) << std::setfill('0') << _score;
		std::string score = ss.str();
		_fontRenderer.draw(glm::vec2(10, 10), score, 20.f);

		for (int i = 0; i < _lives; i++)
		{
			PhysicsComponent *component = const_cast<PhysicsComponent*>(_livesRenderer.getPhysicsComponent());
			component->reset(glm::vec2(10.f + i * 20.f, 40.f), glm::vec2(0));
			_livesRenderer.draw();
		}
	}
}
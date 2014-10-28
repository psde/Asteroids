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
		, _stateTime(0.f)
	{
		reset();
	}

	void Game::reset()
	{
		_lives = 3;
		_score = 0;
		_level = 0;
		_state = Game::LevelTransition;

		loadLevel();
	}

	void Game::loadLevel()
	{
		_ship.reset();
		_asteroids.clear();
		for (int i = 0; i < 2 + _level; ++i)
		{
			_asteroids.push_back(new Asteroid(Asteroid::AsteroidSizes().size() - 1));
		}

		if (_asteroids.size() > 0)
		{
			//auto a = _asteroids.at(0);
			//PhysicsComponent *component = const_cast<PhysicsComponent*>(a->getPhysicsComponent());
			//component->reset(glm::vec2(790, 400), glm::vec2(0));
		}
	}

	void Game::destroyAsteroid(Asteroid *asteroid)
	{
		int size = asteroid->getAsteroidSize();
		float asteroidSize = Asteroid::AsteroidSizes().at(size);
		glm::vec2 pos = asteroid->getPhysicsComponent()->getPosition();

		_score += (Asteroid::AsteroidSizes().size() - size) * 100;

		_emitter.emitParticles(pos + (asteroidSize / 2.f), asteroidSize / 2.f, asteroidSize);

		if (size != 0)
		{
			glm::vec2 dir = glm::normalize(asteroid->getPhysicsComponent()->getVelocity());

			dir = glm::rotate(dir, 0.5f * glm::pi<float>());
			size--;
			_asteroids.push_back(new Asteroid(size, pos, dir));
			_asteroids.push_back(new Asteroid(size, pos, -dir));
		}
	}

	void Game::updateState(float timeDelta)
	{
		_stateTime -= timeDelta;
		if (_state == Game::WaitingForStart || _state == Game::WaitingForRespawn)
		{
			if (_stateTime <= 0.f)
			{
				if (_state == Game::WaitingForRespawn)
				{
					_ship.reset();
					_ship.makeInvincible();
				}
				_state = Game::Playing;
			}
		}
		else if (_state == Game::LevelTransition)
		{
			if (_stateTime <= 0.f)
			{
				_state = Game::WaitingForStart;
				_stateTime = 1.f;
				_level++;
				loadLevel();
			}
		}
		else if (_state == Game::Playing)
		{
			if (_asteroids.size() == 0)
			{
				_state = Game::LevelTransition;
				_stateTime = 2.f;
			}
		}
		else if (_state == Game::Dead)
		{
			if (_lives <= 0)
			{
				_state = Game::GameOver;
			}
			else
			{
				_lives--;
				_state = Game::WaitingForRespawn;
				_stateTime = 2.f;
			}
		}
		else if (_state == Game::GameOver)
		{
			if (_window->getGlfwKeyState(GLFW_KEY_ENTER) == GLFW_PRESS || _window->getGlfwKeyState(GLFW_KEY_KP_ENTER) == GLFW_PRESS)
			{
				reset();
			}
		}
	}

	void Game::update(float timeDelta)
	{
		// Cheats!
		if (_window->getGlfwKeyState(GLFW_KEY_DELETE) == GLFW_PRESS)
		{
			_lives = 3;
			_asteroids.clear();
		}

		updateState(timeDelta);
		if (_state == Game::Playing || _state == Game::LevelTransition)
		{
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
			_ship.rotate(rotation);
		}

		//PhysicsComponent *component = const_cast<PhysicsComponent*>(_ship.getPhysicsComponent());
		//component->reset(_window->getCursorPosition() - glm::vec2(10), glm::vec2(0));

		if (_state != Game::GameOver && _state != Game::WaitingForRespawn && _state != Game::WaitingForStart)
		{
			_ship.update(timeDelta);
		}

		if (_state != Game::WaitingForStart)
		{
			_emitter.update(timeDelta);

			for (auto asteroid : _asteroids)
			{
				asteroid->update(timeDelta);
			}

			for (auto p : _projectiles)
			{
				p->update(timeDelta);
			}

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
						removeProjectile = true;

						if ((*projectileIterator)->isLaunched())
							removeAsteroid = true;

						(*projectileIterator)->reload();
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

				bool shipCollides = false;
				if (_state == Game::Playing)
					shipCollides = (*asteroidIterator)->getColliderComponent()->collidesWith(_ship.getColliderComponent());

				if (shipCollides)
				{
					if (_ship.isInvincible() == false)
					{
						_state = Game::Dead;
						_emitter.emitParticles(_ship.getPhysicsComponent()->getPosition() + 12.5f, 5, 5);
					}
					removeAsteroid = true;
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
	}

	void Game::draw()
	{
		for (Asteroid* asteroid : _asteroids)
		{
			asteroid->draw();
		}

		if (_state != Game::Dead && _state != Game::WaitingForRespawn && _state != Game::GameOver)
			_ship.draw();

		for (auto p : _projectiles)
		{
			if (p->isLaunched())
				p->draw();
		}

		_emitter.draw();


		if (_state == Game::GameOver)
		{
			_fontRenderer.draw(glm::vec2(220, 260), "GAME OVER", 40.f);
			_fontRenderer.draw(glm::vec2(210, 330), "PRESS RETURN TO PLAY AGAIN", 15.f);
		}
		else
		{
			if (_state == Game::WaitingForStart)
			{
				_fontRenderer.draw(glm::vec2(310, 240), "GET READY", 20.f);
			}
			else if (_state == Game::LevelTransition)
			{
				_fontRenderer.draw(glm::vec2(260, 240), "CONGRATULATIONS", 20.f);
			}

			_fontRenderer.draw(glm::vec2(10, 35), "LIVES", 17.f);
			for (int i = 0; i < _lives; i++)
			{
				PhysicsComponent *component = const_cast<PhysicsComponent*>(_livesRenderer.getPhysicsComponent());
				component->reset(glm::vec2(110.f + i * 15.f, 32.f), glm::vec2(0));
				_livesRenderer.draw();
			}

			std::stringstream scoress;
			scoress << "SCORE " << std::setw(5) << std::setfill('0') << _score;
			std::string score = scoress.str();
			_fontRenderer.draw(glm::vec2(10, 10), score, 17.f);

			std::stringstream levelss;
			levelss << "LEVEL " << std::setw(2) << std::setfill('0') << _level;
			std::string level = levelss.str();
			_fontRenderer.draw(glm::vec2(660, 10), level, 17.f);
		}
	}
}
#include <sstream>
#include <iomanip>
#include <random>
#include <iostream>

#include "Game.h"

#include "Math/Math.h"

#include <GLFW/glfw3.h>

namespace Game
{
	Game::Game(std::shared_ptr<Graphics::Window> window)
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

		// TODO: This will leak memory
		_asteroids.clear();
		_emitter.reset();
		for (int i = 0; i < 2 + _level; ++i)
		{
			_asteroids.push_back(new Asteroid());
		}

		if (_asteroids.size() > 0)
		{
			//auto a = _asteroids.at(0);
			//Components::PhysicsComponent *component = const_cast<Components::PhysicsComponent*>(a->getPhysicsComponent());
			//component->reset(Math::vec2(400, 580), Math::vec2(0));
		}

		_ufo.reset();
		_ufoTime = 20.f;
	}

	void Game::addToScore(int points)
	{
		if (_score + points >= 10000)
		{
			_score -= 10000;
			_lives += 3;
		}
		else
		{
			for (int i = 1000; i < 9000; i+=1000)
			{
				if (_score < i && _score + points >= i)
				{
					_lives++;
				}
			}
		}

		if (_lives > 6)
			_lives = 6;

		_score += points;
	}

	void Game::updateState(float timeDelta)
	{
		_stateTime -= timeDelta;
		switch(_state)
		{
			case WaitingForStart:
			case WaitingForRespawn:
				if (_stateTime <= 0.f)
				{
					if (_state == Game::WaitingForRespawn)
					{
						_ship.reset();
						_ship.makeInvincible();
					}
					_state = Game::Playing;
				}
				break;
			case LevelTransition:
				if (_stateTime <= 0.f)
				{
					_state = Game::WaitingForStart;
					_stateTime = 1.f;
					_level++;
					loadLevel();
				}
				break;
			case Playing:
				if (_asteroids.size() == 0)
				{
					_state = Game::LevelTransition;
					_stateTime = 2.f;
				}
				break;
			case Dead:
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
			case GameOver:
				if (_window->getKeyState(Graphics::KEY_ENTER) == Graphics::KEY_PRESS || _window->getKeyState(Graphics::KEY_KP_ENTER) == Graphics::KEY_PRESS)
				{
					reset();
				}
			default:
				break;
		}
	}

	void Game::resolveCollisions()
	{
		std::vector<std::pair<Asteroid*, bool>> destroyedAsteroids;

		// Resolve Ufo -> Player collision
		if (_ufo &&  _ufo->collidesWith(_ship))
		{
			// Destroy ufo
			_emitter.emitParticles(_ufo->getPhysicsComponent()->getPosition() + 12.5f, 5, 5);
			_ufo = nullptr;

			// Player is dead, set state and release particle cloud
			_state = Game::Dead;
			_emitter.emitParticles(_ship.getPhysicsComponent()->getPosition() + 12.5f, 5, 5);
		}

		// Resolve Asteroids -> Asteroids collisions
		/*for (auto asteroidA : _asteroids)
		{
			if(asteroidA->isDestroyed())
				continue;

			for (auto asteroidB : _asteroids)
			{
				if(asteroidB->isDestroyed() || asteroidA == asteroidB)
					continue;

				bool collides = asteroidA->collidesWith(*asteroidB);

				if(collides)
				{
					asteroidA->destroy();
					destroyedAsteroids.push_back({ asteroidA, false });
					asteroidB->destroy();
					destroyedAsteroids.push_back({ asteroidB, false });
				}
			}
		}*/

		// Resolve Asteroids -> Everything collisions
		for (auto asteroid : _asteroids)
		{
			// If the asteroid is destroyed, we do not need to consider it
			if(asteroid->isDestroyed())
				continue;

			// Resolve Asteroid -> Projectiles collisions
			for(auto projectile : _projectiles)
			{
				// If the projectile is not launched, we do not need to consider it
				if(projectile->isLaunched() == false)
					continue;

				bool collides = projectile->collidesWith(asteroid);
				if(collides)
				{
					// Destroy asteroid, add score if projectile is friendly
					asteroid->destroy();
					destroyedAsteroids.push_back({ asteroid, projectile->isFriendly() });

					projectile->reload();

					// The asteroid was destroyed, none of the oter projectiles can harm it
					break;
				}
			}

			// Did a projectile destroy it?
			if(asteroid->isDestroyed())
				continue;

			if (_ufo && _ufo->collidesWith(asteroid))
			{
				// Destroy Ufo
				_emitter.emitParticles(_ufo->getPhysicsComponent()->getPosition() + 12.5f, 5, 5);
				_ufo = nullptr;

				// Destroy asteroid and don't add to score
				asteroid->destroy();
				destroyedAsteroids.push_back({ asteroid, false });
			}

			// Did the Ufo crash into it?
			if (asteroid->isDestroyed())
				continue;

			// Resolve Asteroid -> Ship collision
			if(_ship.collidesWith(asteroid))
			{
				// Player is dead, set state and release particle cloud
				_state = Game::Dead;
				_emitter.emitParticles(_ship.getPhysicsComponent()->getPosition() + 12.5f, 5, 5);

				// Destroy asteroid and don't add to score
				asteroid->destroy();
				destroyedAsteroids.push_back({ asteroid, false });
			}
		}

		// Resolve Projectile -> Ship/UFO collisions
		for (auto projectile : _projectiles)
		{
			// If the projectile is not launched, we do not need to consider it
			if (projectile->isLaunched() == false)
				continue;

			if (_ufo && projectile->collidesWith(_ufo.get()))
			{
				// Destroy Ufo
				_emitter.emitParticles(_ufo->getPhysicsComponent()->getPosition() + 12.5f, 5, 5);
				_ufo = nullptr;

				// Add to score if projectile originated from player 
				if (projectile->isFriendly())
					addToScore(500);
				
				// Reload projectile
				projectile->reload();
			}

			if (projectile->collidesWith(_ship))
			{
				// Player is dead, set state and release particle cloud
				_state = Game::Dead;
				_emitter.emitParticles(_ship.getPhysicsComponent()->getPosition() + 12.5f, 5, 5);
				projectile->reload();
			}
		}

		for (auto a : destroyedAsteroids)
		{
			destroyAsteroid(a.first, a.second);
		}

		// Delete non-launched projectiles
		for (auto projectile = std::begin(_projectiles); projectile != std::end(_projectiles); )
		{
			if ((*projectile)->isLaunched())
			{
				++projectile;
			}
			else
			{
				projectile = _projectiles.erase(projectile);
			}
		}

		// Delete destroyed asteroids
		for (auto asteroid = std::begin(_asteroids); asteroid != std::end(_asteroids); )
		{
			if ((*asteroid)->isDestroyed())
			{
				asteroid = _asteroids.erase(asteroid);
			}
			else
			{
				++asteroid;
			}
		}
	}

	void Game::destroyAsteroid(Asteroid *asteroid, bool addPoints)
	{
		int size = asteroid->getAsteroidSize();
		float asteroidSize = Asteroid::AsteroidSizes().at(size);
		Math::vec2 pos = asteroid->getPhysicsComponent()->getPosition() + (asteroidSize / 2.f);

		// Add to score if needed
		if (addPoints)
			addToScore((Asteroid::AsteroidSizes().size() - size) * 100);

		// Emitt particle cloud
		_emitter.emitParticles(pos, asteroidSize / 2.f, static_cast<int>(asteroidSize));

		// Flag asteroid as destroyed
		asteroid->destroy();

		// If the asteroid is not the smallest one yet, spawn two new ones
		if (size != 0)
		{
			size--;
			asteroidSize = Asteroid::AsteroidSizes().at(size);
			pos -= asteroidSize / 2.f;
			Math::vec2 dir = Math::normalize(asteroid->getPhysicsComponent()->getVelocity());

			dir = Math::rotate(dir, 0.5f * Math::pi<float>());
			_asteroids.push_back(new Asteroid(size, pos + (dir * asteroidSize / 2.f), dir));
			_asteroids.push_back(new Asteroid(size, pos + (-dir * asteroidSize / 2.f), -dir));
		}

		// Delete asteroid
		delete asteroid;
	}

	void Game::update(float timeDelta)
	{
		// Cheats!
		if (_window->getKeyState(Graphics::KEY_DELETE) == Graphics::KEY_PRESS)
		{
			_lives = 3;
			_asteroids.clear();
		}

		updateState(timeDelta);
		if (_state == Game::Playing || _state == Game::LevelTransition)
		{
			if (_window->getKeyState(Graphics::KEY_UP) == Graphics::KEY_PRESS)
			{
				_ship.accelerate();
			}

			int rotation = 0;
			if (_window->getKeyState(Graphics::KEY_LEFT) == Graphics::KEY_PRESS)
			{
				rotation = -1;
			}
			else if (_window->getKeyState(Graphics::KEY_RIGHT) == Graphics::KEY_PRESS)
			{
				rotation = 1;
			}

			if (_window->getKeyState(Graphics::KEY_SPACE) == Graphics::KEY_PRESS)
			{
				auto projectile = _ship.shoot();

				if (projectile)
				{
					_projectiles.push_back(projectile);
				}
			}
			_ship.rotate(rotation);
		}

		//Components::PhysicsComponent *component = const_cast<Components::PhysicsComponent*>(_ship.getPhysicsComponent());
		//component->reset(_window->getCursorPosition() - Math::vec2(10), Math::vec2(0));

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

			if (_ufo)
			{
				_ufo->update(timeDelta);
				auto projectile = _ufo->shoot();

				if (projectile)
				{
					_projectiles.push_back(projectile);
				}
			}
			else
			{
				_ufoTime -= timeDelta;

				if (_ufoTime <= 0.f)
				{
					_ufo.reset(new UFO());
					_ufoTime = 20.f;
				}
			}

			resolveCollisions();
		}
	}

	void Game::draw()
	{
		for (Asteroid* asteroid : _asteroids)
		{
			asteroid->draw();
		}

		if(_ufo)
			_ufo->draw();

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
			_fontRenderer.draw(Math::vec2(220, 260), "GAME OVER", 40.f);
			_fontRenderer.draw(Math::vec2(210, 330), "PRESS RETURN TO PLAY AGAIN", 15.f);
		}
		else
		{
			if (_state == Game::WaitingForStart)
			{
				_fontRenderer.draw(Math::vec2(310, 240), "GET READY", 20.f);
			}
			else if (_state == Game::LevelTransition)
			{
				_fontRenderer.draw(Math::vec2(260, 240), "CONGRATULATIONS", 20.f);
			}

			_fontRenderer.draw(Math::vec2(10, 35), "LIVES", 17.f);
			for (int i = 0; i < _lives; i++)
			{
				Components::PhysicsComponent *component = const_cast<Components::PhysicsComponent*>(_livesRenderer.getPhysicsComponent());
				component->reset(Math::vec2(110.f + i * 15.f, 32.f), Math::vec2(0));
				_livesRenderer.draw();
			}

			std::stringstream scoress;
			scoress << "SCORE " << std::setw(4) << std::setfill('0') << _score;
			std::string score = scoress.str();
			_fontRenderer.draw(Math::vec2(10, 10), score, 17.f);

			std::stringstream levelss;
			levelss << "LEVEL " << std::setw(2) << std::setfill('0') << _level;
			std::string level = levelss.str();
			_fontRenderer.draw(Math::vec2(660, 10), level, 17.f);
		}
	}
}
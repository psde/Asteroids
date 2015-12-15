#include <sstream>
#include <iomanip>
#include <random>
#include <iostream>
#include <algorithm>

#include "Game.h"

#include "Math/Math.h"

#include "Graphics/Graphics.h"
#include "Graphics/Window/Window.h"
#include "Graphics/Shader/Globals.h"

namespace Game
{
	Game::Game()
		: _window(&Graphics::Window::instance())
		, _livesRenderer(22.5f)
		, _stateTime(0.f)
	{
		reset();
		loop();
	}

	void Game::loop()
	{
		float last_frametime = 0.f;
		const float frametime_max = 1.0f;
		float accumulatedDrawingTime = 0.0;
		int frames = 0;

		// Physics simulation
		float accumulatedPhysicsTime = 0.f;
		const float physicsTimeStep = 0.001f;
		int physicSteps = 0;
		const int maxPhysicsStepsPerFrame = 15;

		auto startTime = Graphics::getTime();
		auto endTime = startTime;

		while (!_window->shouldClose())
		{
			float timeDelta = endTime - startTime;
			startTime = Graphics::getTime();

			// Perform physics simulation steps
			physicSteps = 0;
			accumulatedPhysicsTime += timeDelta;
			while (accumulatedPhysicsTime >= physicsTimeStep)
			{
				update(physicsTimeStep);
				accumulatedPhysicsTime -= physicsTimeStep;
				physicSteps++;

				// Do not freeze the game when physics simulation can't keep up
				// but slow down the game instead
				if (physicSteps >= maxPhysicsStepsPerFrame)
					break;
			}

			Graphics::ShaderGlobals::update<float>("time", endTime);
			auto ratio = _window->windowDimensions().x / 800.0;
			Graphics::ShaderGlobals::update<Math::vec2>("windowDimensions", _window->windowDimensions() / ratio);

			draw();

			accumulatedDrawingTime += timeDelta;
			frames++;
			if (endTime > last_frametime + frametime_max)
			{
				std::cout << "time/frame: " << (accumulatedDrawingTime / (double)frames) * 1000.0f << "ms (" << frames << " frames, " << physicSteps << " physic steps)" << std::endl;
				last_frametime = endTime;
				accumulatedDrawingTime = 0.0f;
				frames = 0;
			}

			_window->finishFrame();
			endTime = Graphics::getTime();
		}
	}

	void Game::reset()
	{
		_level = 0;
		_state = Game::LevelTransition;
		_ship.resetLives();

		loadLevel();
	}

	void Game::loadLevel()
	{
		_ship.resetPosition();
		_asteroids.clear();
		_emitter.reset();
		for (int i = 0; i < 2 + _level; ++i)
		{
			_asteroids.push_back(std::make_shared<Asteroid>());
		}

		_ufo.reset();
		_ufoTime = 20.f;
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
						_ship.resetPosition();
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
				if (_ship.getLives() <= 0)
				{
					_state = Game::GameOver;
				}
				else
				{
					_ship.removeLive();
					_state = Game::WaitingForRespawn;
					_stateTime = 2.f;
				}
			case GameOver:
				if (_window->getKeyState(Graphics::KEY_ENTER) == Graphics::KeyState::Press
					|| _window->getKeyState(Graphics::KEY_KP_ENTER) == Graphics::KeyState::Press)
				{
					reset();
				}
			default:
				break;
		}
	}

	void Game::resolveCollisions()
	{
		std::vector<std::pair<std::shared_ptr<Asteroid>, bool>> destroyedAsteroids;

		// Resolve Ufo -> Player collision
		if (_ufo &&  _ufo->collidesWith(_ship))
		{
			// Destroy ufo
			_emitter.emitParticles(_ufo->getPhysicsComponent()->getPosition() + 12.5f, 5, 5);
			_ufo = nullptr;

			if (_ship.isInvincible() == false)
			{
				// Player is dead, set state and release particle cloud
				_state = Game::Dead;
				_emitter.emitParticles(_ship.getPhysicsComponent()->getPosition() + 12.5f, 5, 5);
			}
		}

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

				bool collides = projectile->collidesWith(asteroid.get());
				if(collides)
				{
					// Destroy asteroid, add score if projectile is friendly
					asteroid->destroy();
					destroyedAsteroids.push_back({ asteroid, projectile->isFriendly() });

					projectile->reload();

					// The asteroid was destroyed, none of the other projectiles can harm it
					break;
				}
			}

			// Did a projectile destroy it?
			if(asteroid->isDestroyed())
				continue;

			if (_ufo && _ufo->collidesWith(asteroid.get()))
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
			if (_state == Game::Playing && _ship.collidesWith(asteroid.get()))
			{
				if (_ship.isInvincible() == false)
				{
					// Player is dead, set state and release particle cloud
					_state = Game::Dead;
					_emitter.emitParticles(_ship.getPhysicsComponent()->getPosition() + 12.5f, 5, 5);
				}

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
				_emitter.emitParticles(_ufo->getPhysicsComponent()->getPosition() + 12.5f, 5, 5 );
				_ufo = nullptr;

				// Add to score if projectile originated from player 
				if (projectile->isFriendly())
					_ship.addScore(500);
				
				// Reload projectile
				projectile->reload();
			}

			if (_state == Game::Playing && projectile->collidesWith(_ship))
			{
				if (_ship.isInvincible() == false)
				{
					// Player is dead, set state and release particle cloud
					_state = Game::Dead;
					_emitter.emitParticles(_ship.getPhysicsComponent()->getPosition() + 12.5f, 5, 5);
				}
				projectile->reload();
			}
		}

		for (auto a : destroyedAsteroids)
		{
			destroyAsteroid(a.first, a.second);
		}

		// Delete non-launched projectiles
		_projectiles.erase(std::remove_if(std::begin(_projectiles),
										  std::end(_projectiles),
										  [](const std::shared_ptr<Projectile>& d){ return !d->isLaunched(); }),
						   std::end(_projectiles));

		// Delete destroyed asteroids
		_asteroids.erase(std::remove_if(std::begin(_asteroids),
										std::end(_asteroids),
										[](const std::shared_ptr<Asteroid>& d){ return d->isDestroyed(); }),
						 std::end(_asteroids));
	}

	void Game::destroyAsteroid(std::shared_ptr<Asteroid>& asteroid, bool addPoints)
	{
		int size = asteroid->getAsteroidSize();
		float asteroidSize = Asteroid::AsteroidSizes().at(size);
		Math::vec2 pos = asteroid->getPhysicsComponent()->getPosition() + (asteroidSize / 2.f);

		// Add to score if needed
		if (addPoints)
			_ship.addScore((Asteroid::AsteroidSizes().size() - size) * 100);

		// Emit particle cloud
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
			_asteroids.push_back(std::make_shared<Asteroid>(size, pos + (dir * asteroidSize / 2.f), dir));
			_asteroids.push_back(std::make_shared<Asteroid>(size, pos + (-dir * asteroidSize / 2.f), -dir));
		}
	}

	void Game::update(float timeDelta)
	{
		// Cheats!
		if (_window->getKeyState(Graphics::KEY_DELETE) == Graphics::KeyState::Press)
		{
			//_lives = 3;
			_asteroids.clear();
		}

		updateState(timeDelta);

		// Input
		if (_state == Game::Playing || _state == Game::LevelTransition)
		{
			auto projectile = _ship.handleInput();
			if (projectile)
			{
				_projectiles.push_back(projectile);
			}
		}

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
		for (auto asteroid : _asteroids)
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
			for (unsigned int i = 0; i < _ship.getLives(); i++)
			{
				Components::PhysicsComponent *component = const_cast<Components::PhysicsComponent*>(_livesRenderer.getPhysicsComponent());
				component->reset(Math::vec2(110.f + i * 15.f, 32.f), Math::vec2(0));
				_livesRenderer.draw();
			}

			std::stringstream scoress;
			scoress << "SCORE " << std::setw(4) << std::setfill('0') << _ship.getScore();
			std::string score = scoress.str();
			_fontRenderer.draw(Math::vec2(10, 10), score, 17.f);

			std::stringstream levelss;
			levelss << "LEVEL " << std::setw(2) << std::setfill('0') << _level;
			std::string level = levelss.str();
			_fontRenderer.draw(Math::vec2(660, 10), level, 17.f);
		}
	}
}
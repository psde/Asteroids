#include <sstream>
#include <iomanip>
#include <random>
#include <iostream>
#include <algorithm>

#include "Game.h"

#include "Math/Math.h"

#include "Graphics/Window/Window.h"
#include "Graphics/Shader/Globals.h"

namespace Game
{
	Game::Game()
		: m_window(Graphics::Window::instance())
		, m_emitter(ParticleEmitter::instance())
		, m_fontRenderer(FontRenderer::instance())
		, m_remainingLives(22.5f)
		, m_stateTime(0.f)
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

		auto startTime = m_window.time();
		auto endTime = startTime;

		while (!m_window.shouldClose())
		{
			float timeDelta = endTime - startTime;
			startTime = m_window.time();

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
			auto dimensions = m_window.windowDimensions();
			auto ratio = dimensions.x / 800.0;
			Graphics::ShaderGlobals::update<Math::vec2>("windowDimensions", dimensions / ratio);

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

			m_window.finishFrame();
			endTime = m_window.time();
		}
	}

	void Game::reset()
	{
		m_level = 0;
		m_state = Game::LevelTransition;
		m_ship.resetLives();

		loadLevel();
	}

	void Game::loadLevel()
	{
		m_ship.resetPosition();
		m_asteroids.clear();
		m_emitter.reset();
		for (decltype(m_level) i = 0; i < 2 + m_level; ++i)
		{
			m_asteroids.push_back(std::make_shared<Asteroid>());
		}

		m_ufo.reset();
		m_ufoTime = 20.f;
	}

	void Game::updateState(float timeDelta)
	{
		m_stateTime -= timeDelta;
		switch(m_state)
		{
			case WaitingForStart:
			case WaitingForRespawn:
				if (m_stateTime <= 0.f)
				{
					if (m_state == Game::WaitingForRespawn)
					{
						m_ship.resetPosition();
						m_ship.makeInvincible();
					}
					m_state = Game::Playing;
				}
				break;
			case LevelTransition:
				if (m_stateTime <= 0.f)
				{
					m_state = Game::WaitingForStart;
					m_stateTime = 1.f;
					m_level++;
					loadLevel();
				}
				break;
			case Playing:
				if (m_asteroids.size() == 0)
				{
					m_state = Game::LevelTransition;
					m_stateTime = 2.f;
				}
				break;
			case Dead:
				if (m_ship.lives() <= 0)
				{
					m_state = Game::GameOver;
				}
				else
				{
					m_ship.removeLive();
					m_state = Game::WaitingForRespawn;
					m_stateTime = 2.f;
				}
			case GameOver:
				if (m_window.getKeyState(Graphics::KEY_ENTER) == Graphics::KeyState::Press
					|| m_window.getKeyState(Graphics::KEY_KP_ENTER) == Graphics::KeyState::Press)
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
		if (m_ufo && m_ufo->collidesWith(m_ship))
		{
			// Destroy ufo
			m_ufo->destroy();
			m_ufo.reset(nullptr);

			if (m_ship.isInvincible() == false)
			{
				// Player is dead, set state and release particle cloud
				m_state = Game::Dead;
				m_ship.destroy();
			}
		}

		// Resolve Asteroids -> Everything collisions
		for (auto asteroid : m_asteroids)
		{
			// If the asteroid is destroyed, we do not need to consider it
			if(asteroid->isDestroyed())
				continue;

			// Resolve Asteroid -> Projectiles collisions
			for(auto projectile : m_projectiles)
			{
				// If the projectile is not launched, we do not need to consider it
				if(projectile->launched() == false)
					continue;

				bool collides = projectile->collidesWith(asteroid.get());
				if(collides)
				{
					// Destroy asteroid, add score if projectile is friendly
					asteroid->destroy();
					destroyedAsteroids.push_back({ asteroid, projectile->friendly() });

					projectile->reload();

					// The asteroid was destroyed, none of the other projectiles can harm it
					break;
				}
			}

			// Did a projectile destroy it?
			if(asteroid->isDestroyed())
				continue;

			if (m_ufo && m_ufo->collidesWith(asteroid.get()))
			{
				// Destroy Ufo
				m_ufo->destroy();
				m_ufo.reset(nullptr);

				// Destroy asteroid and don't add to score
				asteroid->destroy();
				destroyedAsteroids.push_back({ asteroid, false });
			}

			// Did the Ufo crash into it?
			if (asteroid->isDestroyed())
				continue;

			// Resolve Asteroid -> Ship collision
			if (m_state == Game::Playing && m_ship.collidesWith(asteroid.get()))
			{
				if (m_ship.isInvincible() == false)
				{
					// Player is dead, set state and release particle cloud
					m_state = Game::Dead;
					m_ship.destroy();
				}

				// Destroy asteroid and don't add to score
				asteroid->destroy();
				destroyedAsteroids.push_back({ asteroid, false });
			}
		}

		// Resolve Projectile -> Ship/UFO collisions
		for (auto projectile : m_projectiles)
		{
			// If the projectile is not launched, we do not need to consider it
			if (projectile->launched() == false)
				continue;

			if (m_ufo && projectile->collidesWith(m_ufo.get()))
			{
				// Destroy Ufo
				m_ufo->destroy();
				m_ufo.reset(nullptr);

				// Add to score if projectile originated from player 
				if (projectile->friendly())
					m_ship.addScore(500);
				
				// Reload projectile
				projectile->reload();
			}

			if (m_state == Game::Playing && projectile->collidesWith(m_ship))
			{
				if (m_ship.isInvincible() == false)
				{
					// Player is dead, set state and release particle cloud
					m_state = Game::Dead;
					m_ship.destroy();
				}
				projectile->reload();
			}
		}

		for (auto a : destroyedAsteroids)
		{
			destroyAsteroid(a.first, a.second);
		}

		// Delete non-launched projectiles
		m_projectiles.erase(std::remove_if(std::begin(m_projectiles),
										   std::end(m_projectiles),
										   [](const std::shared_ptr<Projectile>& d){ return !d->launched(); }),
							std::end(m_projectiles));

		// Delete destroyed asteroids
		m_asteroids.erase(std::remove_if(std::begin(m_asteroids),
										 std::end(m_asteroids),
										 [](const std::shared_ptr<Asteroid>& d){ return d->isDestroyed(); }),
						  std::end(m_asteroids));
	}

	void Game::destroyAsteroid(std::shared_ptr<Asteroid>& asteroid, bool addPoints)
	{
		int size = asteroid->asteroidSize();
		float asteroidSize = Asteroid::AsteroidSizes().at(size);
		Math::vec2 pos = asteroid->physicsComponent()->getPosition() + (asteroidSize / 2.f);

		// Add to score if needed
		if (addPoints)
			m_ship.addScore((Asteroid::AsteroidSizes().size() - size) * 100);

		// Flag asteroid as destroyed
		asteroid->destroy();

		// If the asteroid is not the smallest one yet, spawn two new ones
		if (size != 0)
		{
			size--;
			asteroidSize = Asteroid::AsteroidSizes().at(size);
			pos -= asteroidSize / 2.f;
			Math::vec2 dir = Math::normalize(asteroid->physicsComponent()->getVelocity());

			dir = Math::rotate(dir, 0.5f * Math::pi<float>());
			m_asteroids.push_back(std::make_shared<Asteroid>(size, pos + (dir * asteroidSize / 2.f), dir));
			m_asteroids.push_back(std::make_shared<Asteroid>(size, pos + (-dir * asteroidSize / 2.f), -dir));
		}
	}

	void Game::update(float timeDelta)
	{
		// Cheats!
		if (m_window.getKeyState(Graphics::KEY_DELETE) == Graphics::KeyState::Press)
		{
			//m_lives = 3;
			m_asteroids.clear();
		}

		updateState(timeDelta);

		// Input
		if (m_state == Game::Playing || m_state == Game::LevelTransition)
		{
			auto projectile = m_ship.handleInput();
			if (projectile)
			{
				m_projectiles.push_back(projectile);
			}
		}

		if (m_state != Game::GameOver && m_state != Game::WaitingForRespawn && m_state != Game::WaitingForStart)
		{
			m_ship.update(timeDelta);
		}

		if (m_state != Game::WaitingForStart)
		{
			m_emitter.update(timeDelta);
			
			for (auto asteroid : m_asteroids)
			{
				asteroid->update(timeDelta);
			}

			for (auto p : m_projectiles)
			{
				p->update(timeDelta);
			}

			if (m_ufo)
			{
				m_ufo->update(timeDelta);
				auto projectile = m_ufo->shoot();

				if (projectile)
				{
					m_projectiles.push_back(projectile);
				}
			}
			else
			{
				m_ufoTime -= timeDelta;

				if (m_ufoTime <= 0.f)
				{
					m_ufo.reset(new UFO());
					m_ufoTime = 20.f;
				}
			}

			resolveCollisions();
		}
	}

	void Game::draw()
	{
		for (auto asteroid : m_asteroids)
		{
			asteroid->draw();
		}

		if(m_ufo)
			m_ufo->draw();

		if (m_state != Game::Dead && m_state != Game::WaitingForRespawn && m_state != Game::GameOver)
			m_ship.draw();

		for (auto p : m_projectiles)
		{
			if (p->launched())
				p->draw();
		}

		m_emitter.draw();


		if (m_state == Game::GameOver)
		{
			m_fontRenderer.draw(Math::vec2(220, 260), "GAME OVER", 40.f);
			m_fontRenderer.draw(Math::vec2(210, 330), "PRESS RETURN TO PLAY AGAIN", 15.f);
		}
		else
		{
			if (m_state == Game::WaitingForStart)
			{
				m_fontRenderer.draw(Math::vec2(310, 240), "GET READY", 20.f);
			}
			else if (m_state == Game::LevelTransition)
			{
				m_fontRenderer.draw(Math::vec2(260, 240), "CONGRATULATIONS", 20.f);
			}

			m_fontRenderer.draw(Math::vec2(10, 35), "LIVES", 17.f);
			for (unsigned int i = 0; i < m_ship.lives(); i++)
			{
				Components::PhysicsComponent *component = const_cast<Components::PhysicsComponent*>(m_remainingLives.physicsComponent());
				component->reset(Math::vec2(110.f + i * 15.f, 32.f), Math::vec2(0));
				m_remainingLives.draw();
			}

			std::stringstream scoress;
			scoress << "SCORE " << std::setw(4) << std::setfill('0') << m_ship.score();
			std::string score = scoress.str();
			m_fontRenderer.draw(Math::vec2(10, 10), score, 17.f);

			std::stringstream levelss;
			levelss << "LEVEL " << std::setw(2) << std::setfill('0') << m_level;
			std::string level = levelss.str();
			m_fontRenderer.draw(Math::vec2(660, 10), level, 17.f);
		}
	}
}

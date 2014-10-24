#include <sstream>
#include <iomanip>
#include <random>
#include <iostream>

#include "Game.h"

#include <GLFW/glfw3.h>

namespace Game
{
	Game::Game(std::shared_ptr<Window::Window> window)
	: _window(window)
	, _livesRenderer(20.f)
	{
		_lives = 5;
		_score = 123456789;
		for (int i = 0; i < 5; ++i)
		{
			_asteroids.push_back(new Asteroid(Asteroid::AsteroidSizes().size()-1));
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

		//PhysicsComponent *component = const_cast<PhysicsComponent*>(_ship.getPhysicsComponent());
		//component->reset(_window->getCursorPosition(), glm::vec2(0));

		_ship.rotate(rotation);

		_ship.update(timeDelta);

		// Check for collisions!
		for (Asteroid* asteroid : _asteroids)
		{
			bool collides = asteroid->getColliderComponent()->collidesWith(_ship.getColliderComponent());
			if (collides)
			{
				std::cout << "Collides" << std::endl;
			}
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
#include <sstream>
#include <iomanip>
#include <random>

#include "Game.h"

#include <GLFW/glfw3.h>

namespace Game
{
	Game::Game(std::shared_ptr<Window::Window> window)
	: _window(window)
	{
		_score = 123456789;
		for (int i = 0; i < 20; ++i)
		{
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(50, 90);
			_asteroids.push_back(new Asteroid(dis(gen)));
		}
	}

	void Game::update(float timeDelta)
	{
		for (Asteroid* asteroid : _asteroids)
		{
			asteroid->update(timeDelta);
		}
		
		bool moving = _window->getGlfwKeyState(GLFW_KEY_UP) == GLFW_PRESS;
		int rotation = 0;
		if (_window->getGlfwKeyState(GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			rotation = -1;
		}
		else if (_window->getGlfwKeyState(GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			rotation = 1;
		}
		_ship.update(moving, rotation, timeDelta);
	}

	void Game::draw()
	{
		for (Asteroid* asteroid : _asteroids)
		{
			asteroid->draw();
		}

		_ship.draw();

		std::stringstream ss;
		ss << std::setw(12) << std::setfill('0') << _score;
		std::string score = ss.str();
		_fontRenderer.draw(glm::vec2(10, 10), score, 25.f);
	}
}
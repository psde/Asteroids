#include <sstream>
#include <iomanip>
#include <random>

#include "Game.h"

namespace Game
{
	Game::Game()
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

		_ship.update(timeDelta);
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
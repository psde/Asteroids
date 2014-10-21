#pragma once

#include <vector>

#include "Asteroid.h"
#include "Ship.h"
#include "FontRenderer.h"

struct GLFWwindow;

namespace Game
{
	class Game
	{
	private:
		GLFWwindow *_window;

		std::vector<Asteroid*> _asteroids;

		FontRenderer _fontRenderer;

		Ship _ship;

		int _score;

	public:
		Game(GLFWwindow *window);

		void update(float timeDelta);

		void draw();
	};
}
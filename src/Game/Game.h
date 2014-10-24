#pragma once

#include <vector>

#include "Window/Window.h"
#include "Asteroid.h"
#include "Ship.h"
#include "FontRenderer.h"

struct GLFWwindow;

namespace Game
{
	class Game
	{
	private:
		std::shared_ptr<Window::Window> _window;

		std::vector<Asteroid*> _asteroids;

		FontRenderer _fontRenderer;

		Ship _ship;

		int _lives;
		int _score;

	public:
		Game(std::shared_ptr<Window::Window> window);

		void update(float timeDelta);

		void draw();
	};
}
#pragma once

#include <vector>

#include "Window/Window.h"

#include "FontRenderer.h"

#include "Asteroid.h"
#include "Ship.h"
#include "Projectile.h"
#include "ParticleEmitter.h"

struct GLFWwindow;

namespace Game
{
	class Game
	{
	private:
		std::shared_ptr<Window::Window> _window;

		std::vector<Asteroid*> _asteroids;
		std::vector<std::shared_ptr<Projectile>> _projectiles;

		ParticleEmitter _emitter;

		FontRenderer _fontRenderer;

		Ship _ship;
		Ship _livesRenderer;
		
		int _lives;
		int _level;
		int _score;

		void destroyAsteroid(Asteroid *asteroid);
		void loadLevel();

	public:
		Game(std::shared_ptr<Window::Window> window);

		void update(float timeDelta);

		void draw();
	};
}
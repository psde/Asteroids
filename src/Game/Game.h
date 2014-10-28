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
		enum GameState {
			GameOver = 0,
			WaitingForStart,
			Playing,
			Dead,
			WaitingForRespawn,
			LevelTransition
		};

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


		GameState _state;
		float _stateTime;

		void destroyAsteroid(Asteroid *asteroid);
		void loadLevel();
		void updateState(float timeDelta);
		void reset();

	public:
		Game(std::shared_ptr<Window::Window> window);

		void update(float timeDelta);

		void draw();
	};
}
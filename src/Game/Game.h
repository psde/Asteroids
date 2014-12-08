#pragma once

#include <vector>

#include "Graphics/Window/Window.h"

#include "FontRenderer.h"

#include "Asteroid.h"
#include "Ship.h"
#include "UFO.h"
#include "Projectile.h"
#include "ParticleEmitter.h"

struct GLFWwindow;

namespace Game
{
	// Implements game play simulation.
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

		std::shared_ptr<Graphics::Window> _window;

		std::vector<std::shared_ptr<Asteroid>> _asteroids;
		std::vector<std::shared_ptr<Projectile>> _projectiles;
		std::unique_ptr<UFO> _ufo;

		ParticleEmitter _emitter;

		FontRenderer _fontRenderer;

		Ship _ship;
		Ship _livesRenderer;

		float _ufoTime;

		int _lives;
		int _level;
		int _score;

		GameState _state;
		float _stateTime;

		void destroyAsteroid(std::shared_ptr<Asteroid>& asteroid, bool addPoints);
		void loadLevel();
		void updateState(float timeDelta);
		void reset();
		void addToScore(int points);

		void resolveCollisions();

	public:
		Game(std::shared_ptr<Graphics::Window> window);

		void update(float timeDelta);

		void draw();
	};
}
#pragma once

#include <vector>

#include "Graphics/Window/Window.h"

#include "FontRenderer.h"

#include "Asteroid.h"
#include "Ship.h"
#include "UFO.h"
#include "Projectile.h"
#include "ParticleEmitter.h"

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
		} m_state;

		Graphics::Window& m_window;
		ParticleEmitter& m_emitter;
		FontRenderer& m_fontRenderer;

		std::vector<std::shared_ptr<Asteroid>> m_asteroids;
		std::vector<std::shared_ptr<Projectile>> m_projectiles;
		std::unique_ptr<UFO> m_ufo;

		Ship m_ship;
		Ship m_remainingLives;

		float m_ufoTime;

		unsigned int m_level;

		float m_stateTime;

		void destroyAsteroid(std::shared_ptr<Asteroid>& asteroid, bool addPoints);
		void loadLevel();
		void updateState(float timeDelta);
		void reset();
		void addToScore(int points);

		void resolveCollisions();

		void loop();

		void update(float timeDelta);

		void draw();
	public:
		Game();

	};
}
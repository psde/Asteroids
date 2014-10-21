#pragma once

#include <vector>

#include "Asteroid.h"
#include "Ship.h"
#include "FontRenderer.h"

namespace Game
{
	class Game
	{
	private:
		std::vector<Asteroid*> _asteroids;

		FontRenderer _fontRenderer;

		Ship _ship;

		int _score;

	public:
		Game();

		void update(float timeDelta);

		void draw();
	};
}
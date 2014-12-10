#include <iostream>
#include <algorithm>

#include "Math/Math.h"

#include "Game/Game.h"
#include "Graphics/Graphics.h"
#include "Graphics/Window/Window.h"
#include "Graphics/Shader/Globals.h"

int main(int argc, char** argv)
{
	std::shared_ptr<Graphics::Window> window(new Graphics::Window(800, 600));
	Game::Game game(window);

	float last_frametime = 0.f;
	const float frametime_max = 0.1f;
	float accumulatedDrawingTime = 0.0;
	int frames = 0;

	// Physics simulation
	float accumulatedPhysicsTime = 0.f;
	const float physicsTimeStep = 0.001f;
	int physicSteps = 0;
	const int maxPhysicsStepsPerFrame = 30;


	float startTime = Graphics::getTime();
	float endTime = startTime;

	while (!window->shouldClose())
	{
		float timeDelta = endTime - startTime;
		startTime = Graphics::getTime();
		
		// Perform physics simulation steps
		physicSteps = 0;
		accumulatedPhysicsTime += timeDelta;
		while (accumulatedPhysicsTime >= physicsTimeStep)
		{
			game.update(physicsTimeStep);
			accumulatedPhysicsTime -= physicsTimeStep;
			physicSteps++;

			// Do not freeze the game when physics simulation can't keep up
			// but slow down the game instead
			if (physicSteps >= maxPhysicsStepsPerFrame)
				break;
		}

		Graphics::ShaderGlobals::update<float>("time", endTime);
		Graphics::ShaderGlobals::update<Math::vec2>("windowDimensions", window->getWindowDimensions());

		game.draw();

		accumulatedDrawingTime += timeDelta;
		frames++;
		if (endTime > last_frametime + frametime_max)
		{
			std::cout << "time/frame: " << (accumulatedDrawingTime / (double)frames) * 1000.0f << "ms (" << frames << " frames, " << physicSteps << " physic steps)" << std::endl;
			last_frametime = endTime;
			accumulatedDrawingTime = 0.0f;
			frames = 0;
		}

		window->finishFrame();
		endTime = Graphics::getTime();
	}

	return 0;
}
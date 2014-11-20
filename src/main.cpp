#include <iostream>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window/Window.h"
#include "Game/Game.h"
#include "Shader/Globals.h"

int main(int argc, char** argv)
{
	std::shared_ptr<Window::Window> window(new Window::Window(800, 600));
	Game::Game game(window);

	double last_frametime = 0;
	const double frametime_max = 0.1;
	double accumulatedDrawingTime = 0.0;
	int frames = 0;

	// Physics simulation
	double accumulatedPhysicsTime = 0;
	const double physicsTimeStep = 0.001;
	int physicSteps = 0;
	const int maxPhysicsStepsPerFrame = 30;

	while (!window->shouldClose())
	{
		double currentTime = glfwGetTime();
		double timeDelta = window->getTimeDelta();

		// Perform physics simulation steps
		physicSteps = 0;
		accumulatedPhysicsTime += timeDelta;
		while (accumulatedPhysicsTime >= physicsTimeStep)
		{
			game.update(physicsTimeStep);
			accumulatedPhysicsTime -= physicsTimeStep;
			physicSteps++;

			// Do not freeze the game when physics simulation can't keep up
			// but instead slow the game down
			if (physicSteps >= maxPhysicsStepsPerFrame)
				break;
		}

		Shader::Globals::globals().update<float>("time", currentTime);
		Shader::Globals::globals().update<glm::vec2>("windowDimensions", window->getWindowDimensions());

		game.draw();

		accumulatedDrawingTime += timeDelta;
		frames++;
		if (currentTime > last_frametime + frametime_max)
		{
			std::cout << "time/frame: " << (accumulatedDrawingTime / (double)frames) * 1000.0f << "ms (" << frames << " frames, " << physicSteps << " physic steps)" << std::endl;
			last_frametime = currentTime;
			accumulatedDrawingTime = 0.0f;
			frames = 0;
		}

		window->finishFrame();
	}

	return 0;
}
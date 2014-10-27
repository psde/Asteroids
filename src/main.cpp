#include <iostream>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Window/Window.h"
#include "Game/Game.h"
#include "Shader/Globals.h"

static void error_callback(int error, const char* description)
{
	std::cout << "Error: " << error << " " << description << std::endl;
}

int main()
{
	std::shared_ptr<Window::Window> window(new Window::Window(800, 600));
	Game::Game game(window);
	
	double last_frametime = 0;
	const double frametime_max = 0.1;
	double acc_frametime = 0.0;
	int frames = 0;
	double accumulator = 0;
	const double dt = 0.005;
	while (!window->shouldClose())
	{
		double time = glfwGetTime();
		double timeDelta = window->getTimeDelta();

		accumulator += dt;
		while(accumulator > dt)
		{
			game.update(dt);
			accumulator -= dt;
		}

		Shader::Globals::globals().update<float>("time", time);
		Shader::Globals::globals().update<glm::vec2>("windowDimensions", window->getWindowDimensions());

		game.draw();

		//if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		//	glfwSetWindowShouldClose(window, GL_TRUE);

		acc_frametime += timeDelta;
		frames++;
		if (time > last_frametime + frametime_max)
		{
			std::cout << "Frametime: " << (acc_frametime / (double)frames) * 1000.0f << "ms" << std::endl;
			last_frametime = time;
			acc_frametime = 0.0f;
			frames = 0;
		}

		window->finishFrame();
	}
}
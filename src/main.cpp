#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Game/Game.h"
#include "Shader/Globals.h"

static void error_callback(int error, const char* description)
{
	std::cout << "Error: " << error << " " << description << std::endl;
}

int main()
{
	GLFWwindow* window;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	window = glfwCreateWindow(800, 600, "Asteroids!", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}

	Game::Game game(window);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	//float ratio; = width / (float)height;

	glViewport(0, 0, width, height);

	double time = glfwGetTime();
	double timeDelta = 0;
	
	double last_frametime = time;
	const double frametime_max = 0.1;
	double acc_frametime = 0.0;
	int frames = 0;
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		double oldTime = time;
		time = glfwGetTime();
		timeDelta = time - oldTime;

		game.update(timeDelta);

		Shader::Globals::globals().update<float>("time", time);
		Shader::Globals::globals().update<glm::vec2>("windowDimensions", glm::vec2(width, height));

		game.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		acc_frametime += timeDelta;
		frames++;
		if (time > last_frametime + frametime_max)
		{
			std::cout << "Frametime: " << (acc_frametime / (double)frames) * 1000.0f << "ms" << std::endl;
			last_frametime = time;
			acc_frametime = 0.0f;
			frames = 0;
		}
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
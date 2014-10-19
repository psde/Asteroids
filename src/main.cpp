#include <iostream>
#include <sstream>
#include <iomanip>
#include <memory>
#include <random>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader/Shader.h"
#include "Geometry/Mesh.h"
#include "Game/Asteroid.h"
#include "Game/Ship.h"
#include "Game/FontRenderer.h"

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
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

	Game::FontRenderer fontRenderer;

	Game::Ship ship;

	std::vector<Game::Asteroid*> asteroids;

	for (int i = 0; i < 20; ++i)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(50, 90);
		asteroids.push_back(new Game::Asteroid(dis(gen)));
	}
	
	std::stringstream ss;
	ss << std::setw(12) << std::setfill('0') << 123456789;
	std::string score = ss.str();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	float ratio;
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	ratio = width / (float)height;

	glViewport(0, 0, width, height);

	float factor = 0.0f;
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
		
		for (Game::Asteroid* asteroid : asteroids)
		{
			asteroid->update((float)timeDelta);
			asteroid->draw();
		}

		ship.update((float)timeDelta);
		ship.draw();

		fontRenderer.draw(glm::vec2(10, 10), score, 25.f);

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
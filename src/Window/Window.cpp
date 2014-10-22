#include <iostream>

#include "Window.h"

namespace Window
{
	namespace {
		static void error_callback(int error, const char* description)
		{
			std::cout << "Error: " << error << " " << description << std::endl;
		}
	}

	Window::Window(int width, int height)
	: _time(0.0f)
	{
		glfwSetErrorCallback(error_callback);

		if (!glfwInit())
			exit(EXIT_FAILURE);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint(GLFW_SAMPLES, 8);

		_glfwWindow = glfwCreateWindow(width, height, "Asteroids!", nullptr, nullptr);
		if (!_glfwWindow)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		
		glfwMakeContextCurrent(_glfwWindow);

		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			exit(EXIT_FAILURE);
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		glm::ivec2 dimensions(getWindowDimensions());
		glViewport(0, 0, dimensions.x, dimensions.y);
	}

	Window::~Window()
	{
		glfwDestroyWindow(_glfwWindow);
		glfwTerminate();
	}

	glm::ivec2 Window::getWindowDimensions()
	{
		glm::ivec2 dimensions;
		glfwGetWindowSize(_glfwWindow, &dimensions.x, &dimensions.y);
		//float ratio; = width / (float)height;
		return dimensions;
	}

	bool Window::shouldClose()
	{
		return glfwWindowShouldClose(_glfwWindow);
	}

	void Window::finishFrame()
	{
		glfwSwapBuffers(_glfwWindow);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		_time.y = _time.x;
		_time.x = glfwGetTime();
	}

	float Window::getTimeDelta()
	{
		return _time.x - _time.y;
	}

	int Window::getGlfwKeyState(int key)
	{
		return glfwGetKey(_glfwWindow, key);
	}
}
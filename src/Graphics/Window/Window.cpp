#include <iostream>

#include "Window.h"

namespace Graphics
{
	namespace {
		static void error_callback(int error, const char* description)
		{
			std::cout << "Error: " << error << " " << description << std::endl;
		}
	}

	Window::Window(int width, int height)
	{
		glfwSetErrorCallback(error_callback);

		if (!glfwInit())
			exit(EXIT_FAILURE);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		glfwWindowHint(GLFW_SAMPLES, 8);

		m_glfwWindow = glfwCreateWindow(width, height, "Asteroids!", nullptr, nullptr);
		glfwSetWindowAspectRatio(m_glfwWindow, 4, 3);
		if (!m_glfwWindow)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(m_glfwWindow);

		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			exit(EXIT_FAILURE);
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		Math::vec2 dimensions(framebufferDimensions());
		glViewport(0, 0, static_cast<GLsizei>(dimensions.x), static_cast<GLsizei>(dimensions.y));

		glfwSetWindowSizeCallback(m_glfwWindow, [](GLFWwindow* window, int, int)
		{
			int x, y;
			glfwGetFramebufferSize(window, &x, &y);
			glViewport(0, 0, x, y);
		});
	}

	Window& Window::instance()
	{
		static Window instance(800, 600);
		return instance;
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_glfwWindow);
		glfwTerminate();
	}

	Math::vec2 Window::windowDimensions() const
	{
		int x, y;
		glfwGetWindowSize(m_glfwWindow, &x, &y);
		return Math::vec2(x, y);
	}

	Math::vec2 Window::framebufferDimensions() const
	{
		int x, y;
		glfwGetFramebufferSize(m_glfwWindow, &x, &y);
		return Math::vec2(x, y);
	}

	Math::vec2 Window::cursorPosition() const
	{
		double x, y;
		glfwGetCursorPos(m_glfwWindow, &x, &y);
		return Math::vec2(x, y);
	}

	bool Window::shouldClose() const
	{
		return (glfwWindowShouldClose(m_glfwWindow) != 0);
	}

	void Window::finishFrame()
	{
		glfwSwapBuffers(m_glfwWindow);
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (static_cast<int>(getKeyState(KEY_ESCAPE)) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_glfwWindow, GL_TRUE);
	}
	
	KeyState Window::getKeyState(Key key)
	{
		return static_cast<KeyState>(glfwGetKey(m_glfwWindow, key));
	}

	float Window::time()
	{
		return static_cast<float>(glfwGetTime());
	}
}

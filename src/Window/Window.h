#pragma once

#include <memory>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

namespace Window
{
	class Window
	{
	private:

		glm::vec2 _time;

	public:
		GLFWwindow *_glfwWindow;

		Window(int width, int height);
		~Window();

		glm::ivec2 getWindowDimensions();
		bool shouldClose();

		void finishFrame();

		float getTimeDelta();

		int getGlfwKeyState(int key);
	};
}
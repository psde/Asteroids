#pragma once

#include <memory>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//#include <glm/glm.hpp>
#include "Math/Math.h"

namespace Window
{
	class Window
	{
	private:
		GLFWwindow *_glfwWindow;

		glm::vec2 _time;

	public:
		Window(int width, int height);
		~Window();

		glm::vec2 getWindowDimensions();
		glm::vec2 getCursorPosition();
		bool shouldClose();

		void finishFrame();

		float getTimeDelta();

		int getGlfwKeyState(int key);
	};
}
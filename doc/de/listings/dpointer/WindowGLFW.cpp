#include "Window.h"

struct WindowDetail
{
	GLFWwindow *window;
} 

Window::Window(unsigned int width, unsigned int height)
: d_ptr(new WindowDetail)
{
	d_ptr->window = glfwCreateWindow(width, height, ...);
}

// ...
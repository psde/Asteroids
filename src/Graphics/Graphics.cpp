#include "Graphics.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Graphics
{
	float getTime()
	{
		return static_cast<float>(glfwGetTime());
	}
}
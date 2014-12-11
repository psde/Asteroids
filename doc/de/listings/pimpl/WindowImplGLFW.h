class WindowImpl
{
private:
	GLFWwindow *window;

public:
	WindowImpl(unsigned int width, unsigned int height)
	{
		window = glfwCreateWindow(width, height, ...);
	}

	vec2 getCursorPosition()
	{
		// ...
	}

	KeyState getKeyState(Key key)
	{
		// ...
	}
}
class Window
{
private:
	GLFWwindow *_glfwWindow;
	
public:
	Window(int width, int height);
	~Window();

	vec2 getWindowDimensions();
	vec2 getCursorPosition();
	bool shouldClose();

	void finishFrame();
	
	KeyState getKeyState(Key key);
};
class Window
{
public:
	Window(unsigned int width, unsigned int height);

	vec2 getCursorPosition();
	KeyState getKeyState(Key key);
private:
	class WindowImpl;
	std::unique_ptr<WindowImpl> impl;
}
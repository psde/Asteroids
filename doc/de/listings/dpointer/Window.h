class Window
{
public:
	Window(unsigned int width, unsigned int height);

	vec2 getCursorPosition();
	KeyState getKeyState(Key key);
private:
	struct WindowDetail;
	std::unique_ptr<WindowDetail> d_ptr;
}
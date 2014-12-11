#include "Window.h"

#ifdef USE_GLFW
#include "WindowImplGLFW.h"
#elif USE_MANTLE
#include "WindowImplMantle.h"
#elif USE_DIRECTX
#include "WindowImplDirectX.h"
#else
#error No graphic system selected.
#endif

Window::Window(unsigned int width, unsigned int height)
: impl { new WindowImpl(width, height); }
{
	// ...
}

vec2 Window::getCursorPosition()
{
	return impl->getCursorPosition();
}

// ...
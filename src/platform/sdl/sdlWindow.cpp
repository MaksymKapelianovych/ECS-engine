#include "sdlWindow.hpp"
#include "rendering/renderDevice.hpp"
#include <stdexcept>

SDLWindow::SDLWindow(const Application& app,
		int32_t widthIn, int32_t heightIn, const String& title) :
	width(widthIn),
	height(heightIn)
{
	if(!RenderDevice::globalInit()) {
		throw std::runtime_error("Render device could not be initialized");
	}
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
}

SDLWindow::~SDLWindow()
{
	SDL_DestroyWindow(window);
}

WindowHandle SDLWindow::getWindowHandle()
{
	return window;
}

void SDLWindow::changeSize(int32_t width, int32_t height)
{
	// TODO: Implement
}

void SDLWindow::present()
{
	SDL_GL_SwapWindow(window);
}

int32_t SDLWindow::getWidth() const
{
	return width;
}

int32_t SDLWindow::getHeight() const
{
	return height;
}


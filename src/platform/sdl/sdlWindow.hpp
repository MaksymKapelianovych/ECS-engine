#pragma once

#include "core/common.hpp"
#include "core/application.hpp"
#include "dataStructures/string.hpp"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

typedef SDL_Window* WindowHandle;

class SDLWindow
{
public:
	SDLWindow(const Application& app, int32_t width, int32_t height, const String& title);
	virtual ~SDLWindow();

	WindowHandle getWindowHandle();
	void changeSize(int32_t width, int32_t height);
	void present();
	int32_t getWidth() const;
	int32_t getHeight() const;
private:
	WindowHandle window;
	int32_t width;
	int32_t height;
};

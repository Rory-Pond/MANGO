#pragma once

#include <SDL2\SDL.h>
#include <string>

class Window
{
public:
	Window(int width, int height, const std::string& title);
	void Update();

	virtual ~Window();
protected:
private:
	SDL_Window* m_window;
	SDL_GLContext m_glContext;
};


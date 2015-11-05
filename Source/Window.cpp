#include "Window.h"
#include <GL/glew.h>
#include <iostream>

Window::Window(int width, int height, const std::string& title)
{
	// Initialize SDL
	SDL_Init(SDL_INIT_EVERYTHING);

	// Initialize SDL buffers
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create window and set as opengl context
	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	m_glContext = SDL_GL_CreateContext(m_window);

	// Initialize GLEW and check it
	GLenum status = glewInit();
	if (status != GLEW_OK)
		std::cerr << "Glew failed to initialize!" << std::endl;

	//center mouse in window
	SDL_WarpMouseInWindow(m_window, width / 2, height / 2);

	//Intialise OpenGl Values
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.337f, 0.337f, 0.337f, 1.0f);
	glClearDepth(1.0);

}

void Window::Update()
{
	SDL_GL_SwapWindow(m_window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Clear buffer
}


Window::~Window()
{
	SDL_GL_DeleteContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

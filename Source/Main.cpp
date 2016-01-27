
#include <iostream>
#include <vector>

#include "Shader.h"
#include "Window.h"
#include "Arguments.h"
#include "System.h"
#include "Controls.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

int main(int argc, char *argv[])
{
	Arguments Args(argc, argv);

	const int width = 1000, height = 800;
	Window window(width, height, "MANGO");

	System sys(Args.infolder, Args.startspin, Args.numsteps, Args.objectfile);


	//Load Shaders
	GLuint shaderProgram = LoadShaders("./Shader/Instanced.vs", "./Shader/Instanced.fs");
	initUniforms();
	std::cout << "GO" << std::endl;
	do {
		sys.update();
		window.Update();
	} while (HandleEvents(sys));
	
	SDL_Quit();
    return 0;
}


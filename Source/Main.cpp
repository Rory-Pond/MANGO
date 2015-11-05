#include <iostream>
#include <vector>

#include "Shader.h"
#include "Window.h"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

int main(int argc, char * argv[])
{

	const int width = 1000, height = 800;
	Window window(width, height, "MANGO");

	//Load Shaders
	GLuint shaderProgram = LoadShaders("BasicShader.vs", "BasicShader.fs");

	do {
		window.Update();
	} while (1);

    return 0;
}


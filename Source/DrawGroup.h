#pragma once

 #define GLM_FORCE_RADIANS

#include <stdio.h>

#include <vector>
#include <iostream>
#include <list>
#include <cstring>

#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

class DrawGroup
{
public:
	DrawGroup();
	
	virtual ~DrawGroup();
	void update();

	void updateSpinBufffer();
	void OBJloader(std::string file);
	void OBJreader(std::string file);
	void swapSpins(std::vector<glm::vec3>& newSpins);

	void setCoords(std::vector<glm::vec3> in_Coords);

	void addSpins(std::vector<glm::vec3> Spins);

	void genPyramind();
	void genIsosphere();

	void loadBuffers();

	void SpinstepAdd();
	void SpinstepMinus();

	void visble();

	int getCoordsSize()  const { return CoordsSize; } 

private:
	void Buffers();
	
	int currentSpins;

	bool show=true;

	std::list<std::vector<glm::vec3>> AllSpins;
	std::list<std::vector<glm::vec3>>::iterator CurrentSpins;

	std::vector<glm::vec3> Coords, Spins, Vertices, Normals ;
	std::vector<int> Indices;

	int IndicesSize, CoordsSize, VerticesSize, NormalsSize;

	GLuint vertexBuffer, normalBuffer, indexBuffer, coordBuffer, spinBuffer;
	GLuint vao;
};


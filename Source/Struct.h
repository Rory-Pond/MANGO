#pragma once

#include <vector>
#include <glm/glm.hpp>

struct systemInfo {
	int numAtoms;
	int numMaterials;
	std::vector <double> Material;
	glm::vec3 dimensions;
	int MaxSteps;
};

struct spinInfo {
	int StepNum;
	double Time;
	double Field;
	double Tempreture;
	glm::vec3 Magnetisation;
};
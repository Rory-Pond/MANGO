#pragma once

#include "Struct.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <glm/glm.hpp>

// Standard Libraries
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>


class DataInput
{
public:
	DataInput(std::string infolder, int Spin);
	~DataInput();

	systemInfo readHeader();
	spinInfo readHeader(int spinNum);

	std::vector <std::vector<glm::vec3>> getCoords();
	std::vector <std::vector<glm::vec3>> getSpins(int step);

private:
	systemInfo sysInfo;
	std::string folder;
	int initSpin;

	std::vector <std::vector<int>> indexMat;
	void centerCoords(std::vector<glm::vec3>& coords);

	std::vector<glm::vec3> loadCoords();
	std::vector<glm::vec3> loadSpins(int spinFileNum);

	std::vector<std::vector<glm::vec3>> splitData(std::vector<glm::vec3> Data);
};

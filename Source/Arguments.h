#pragma once

#include <string>

class Arguments
{
public:
	Arguments(int argc, char* argv[]);
	~Arguments();
	std::string infolder="./TestData/5nmNDFeB/";
	int startspin=0;
	int numsteps=5;
	std::string objectfile= "./res/OBJ_files/ico-2divitions.waveobj";
private:
	int resolution[2];
	
};
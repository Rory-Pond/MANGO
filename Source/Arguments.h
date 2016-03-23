#pragma once

#include <string>

class Arguments
{
public:
	Arguments(int argc, char* argv[]);
	~Arguments();
	std::string infolder="./";
	int startspin=0;
	int numsteps=0;
	std::string objectfile= "./res/OBJ_files/ico-2divitions.waveobj";
private:
	int resolution[2];
	
};
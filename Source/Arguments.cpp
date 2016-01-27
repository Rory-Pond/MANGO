#include "Arguments.h"

#include <iostream>

Arguments::Arguments(int argc, char* argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		if (std::string(argv[i]) == "-f")  
			infolder = std::string(argv[i+1]);
		if (std::string(argv[i]) == "-start")
			startspin = atoi(argv[i + 1]);
		if (std::string(argv[i]) == "-num")
			numsteps = atoi(argv[i + 1]);
		if (std::string(argv[i]) == "-obj")
			objectfile = std::string(argv[i + 1]);
		if (std::string(argv[i]) == "-test")
			objectfile = "./res/OBJ_files/susan.waveobj";
		if (std::string(argv[i]) == "-cone")
			objectfile = "./res/OBJ_files/triangle_based_cone.obj";
		if (std::string(argv[i]) == "-smvcone")
			objectfile = "./res/OBJ_files/triangle_based_cone_smooth.obj";
		if (std::string(argv[i]) == "-test")
			objectfile = "./res/OBJ_files/susan.waveobj";
	}
}
//ico-1divitions.waveobj
//"./res/OBJ_files/SimpleArrow.obj"
Arguments::~Arguments()
{
}

#pragma once

#include "DrawGroup.h"
#include "DataInput.h"

class System
{
public:
	System(std::string infolder, int startspin, int numsteps, std::string objectfile);
	virtual ~System();

	systemInfo sysInfo;

	void update();

	void increaseStep();
	void decreaseStep();

	void visble(int DrawGroup);

private:
	int currentstep=0;

	void printspininfo(int step);
	std::vector<DrawGroup> materials;
	std::vector<DrawGroup> layers;
	std::vector<spinInfo> spinStepInfo;
};


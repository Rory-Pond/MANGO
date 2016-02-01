#include "System.h"


System::System(std::string infolder,int startspin, int numsteps, std::string objectfile)
{
	glewInit();
	DataInput Data(infolder, startspin);

	//"../Vampire Data/4Fe_MvsT/" 300 steps
	//"../Vampire Data/5nmNDFeB/" 100 steps
	//"./TestData/AndreaFiles/" 60 steps START 2510
	//"../TestData/ECC750K/0.1/" 300 steps
	//"../Vampire Data/HomeMade/" 1 step
	//"../Vampire Data/Simple/" 1 step
	//"../Vampire Data/skerm/"  200 steps BROKEN MATERIAL
	//"../Vampire Data/vortex/
	sysInfo = Data.readHeader();
	std::cout << "Loading system of size: " << sysInfo.numAtoms << std::endl;
	std::cout << "Loading " << numsteps << " timesteps from " << startspin <<" to " << startspin+ numsteps << std::endl;
	spinStepInfo.resize(numsteps);
	//spinStepInfo[0] = Data.readHeader(0);
	
	//=======SKERM FIX=======
	//sysInfo.numMaterials = 1;
	//=======================
	
	materials.resize(sysInfo.numMaterials);

	std::vector<std::vector<glm::vec3>> TempCoords;
	std::vector<std::vector<glm::vec3>> TempSpins;

	TempCoords = Data.getCoords();
	for (int i = 0; i < sysInfo.numMaterials; i++)
	{
		materials[i].setCoords(TempCoords[i]);
	}
	TempCoords.resize(0);

	for (int i = 0; i < numsteps; i++)
	{
		TempSpins = Data.getSpins(i);
		spinStepInfo[i] = Data.readHeader(i);
		for (int j = 0; j < sysInfo.numMaterials; j++)
		{
			materials[j].addSpins(TempSpins[j]);
		}
		TempSpins.resize(0);
	}

	std::cout << "Read in all files" << std::endl;

	for (int i = 0; i < sysInfo.numMaterials; i++)
	{
		materials[i].OBJloader(objectfile);
		materials[i].loadBuffers();
	}


	std::cout << "Materials!" << std::endl;

	printspininfo(currentstep);
}

System::~System()
{
}

void System::update()
{
	for (int i = 0; i < sysInfo.numMaterials; i++)
	{
		materials[i].update();
	}
}

void System::increaseStep()
{

	for (int i = 0; i < sysInfo.numMaterials; i++)
	{
		materials[i].SpinstepAdd();
	}
	if (currentstep != spinStepInfo.size()-1)
		currentstep++;
	printspininfo(currentstep);
}

void System::decreaseStep()
{

	for (int i = 0; i < sysInfo.numMaterials; i++)
	{
		materials[i].SpinstepMinus();
	}
	if (currentstep != 0)
		currentstep--;
	printspininfo(currentstep);
}

void System::visble(int DrawGroup)
{
	materials.at(DrawGroup).visble();
	/* <================================ FIX THIS
	try {
		materials.at(DrawGroup).visble();      // vector::at throws an out-of-range
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
	*/
}

void System::printspininfo(int step)
{
	//std::cout << "Step: " << spinStepInfo[step].StepNum << std::endl;
	//std::cout << "Time: " << spinStepInfo[step].Time << std::endl;
	//std::cout << "Field: " << spinStepInfo[step].Field << std::endl;
	//std::cout << "Magnetisation: " << spinStepInfo[step].Magnetisation.x << spinStepInfo[step].Magnetisation.y << spinStepInfo[step].Magnetisation.z << std::endl;
	//std::cout << "Tempreture: " << spinStepInfo[step].Tempreture << std::endl;
	std::cout << "Field: " << spinStepInfo[step].Field << "\nTime: " << spinStepInfo[step].Time << '\r' << std::flush;
}


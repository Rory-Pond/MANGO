#include "DataInput.h"

DataInput::DataInput(std::string infolder, int Spin)
{
	folder = infolder;
	initSpin = Spin;
	sysInfo = readHeader();
}

DataInput::~DataInput()
{
}

spinInfo DataInput::readHeader(int spinNum)
{
	std::string line;
	spinInfo spiInfo;

	std::stringstream file_sstr;
	file_sstr << folder << "atoms-";
	file_sstr << std::setfill('0') << std::setw(8) << (initSpin + spinNum);
	file_sstr << ".cfg";
	std::string cfg_file = file_sstr.str();
	int counter = 0;

	std::ifstream spinfile;
	spinfile.open(cfg_file.c_str());
	if (spinfile.fail())
	{
		std::cerr << "Error: Opening " << cfg_file.c_str() << " failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "Reading header: " << cfg_file << std::endl;

	// Read in file header
	getline(spinfile, line); // line
	getline(spinfile, line); // Title
	getline(spinfile, line); // line
	getline(spinfile, line); // Date
	getline(spinfile, line); // line
	getline(spinfile, line); // number of atoms
	getline(spinfile, line); // sys dimensions
	getline(spinfile, line); // coord file

	getline(spinfile, line); // time
	line.erase(line.begin(), line.begin() + 5);
	spiInfo.Time = atof(line.c_str());

	getline(spinfile, line); // field
	line.erase(line.begin(), line.begin() + 6);
	spiInfo.Field = atof(line.c_str());

	getline(spinfile, line); // temp
	line.erase(line.begin(), line.begin() + 12);
	spiInfo.Tempreture = atof(line.c_str());

	getline(spinfile, line); // magnetisation
	line.erase(line.begin(), line.begin() + 14);
	std::istringstream ss2(line);
	char const field_delim = '\t';
	std::vector<double> val(0);
	for (std::string num; getline(ss2, num, field_delim);) {
		val.push_back(atof(num.c_str()));
	}
	spiInfo.Magnetisation = glm::vec3{ val[0], val[1], val[2] };

	return spiInfo;
}

systemInfo DataInput::readHeader()
{
	std::string line;
	systemInfo Info;

	std::stringstream file_sstr;
	file_sstr << folder << "atoms-";
	file_sstr << std::setfill('0') << std::setw(8) << initSpin;
	file_sstr << ".cfg";
	std::string cfg_file = file_sstr.str();
	int counter = 0;

	std::ifstream spinfile;
	spinfile.open(cfg_file.c_str());
	if (spinfile.fail())
	{
		std::cerr << "Error: Opening " << cfg_file.c_str() << " failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "Reading header: " << cfg_file << std::endl;

	// Read in file header
	getline(spinfile, line); // line
	getline(spinfile, line); // Title
	getline(spinfile, line); // line
	getline(spinfile, line); // Date
	getline(spinfile, line); // line

	getline(spinfile, line); // number of atoms
	line.erase(line.begin(), line.begin() + 17);
	Info.numAtoms = atoi(line.c_str());

	getline(spinfile, line); // sys dimensions
	char const field_delim = '\t';
	line.erase(line.begin(), line.begin() + 18);
	std::istringstream ss(line);
	std::vector<double> val(0);
	for (std::string num; getline(ss, num, field_delim);) {
		val.push_back(atof(num.c_str()));
	}
	Info.dimensions = glm::vec3{ val[0], val[1], val[2] };

	getline(spinfile, line); // coord file
	getline(spinfile, line); // time
	getline(spinfile, line); // field
	getline(spinfile, line); // temp
	getline(spinfile, line); // magnetisation

	getline(spinfile, line); // number of materials
	line.erase(line.begin(), line.begin() + 20);
	Info.numMaterials = atoi(line.c_str());
	for (int i = 0; i < Info.numMaterials; i++) {
		getline(spinfile, line); //material
		Info.Material.push_back(atof(line.c_str()));
	}

	return Info;
}

std::vector<std::vector<glm::vec3>> DataInput::getCoords()
{
	return splitData(loadCoords());
}

std::vector<std::vector<glm::vec3>> DataInput::getSpins(int step)
{
	return splitData(loadSpins(step));
}

void DataInput::centerCoords(std::vector<glm::vec3>& coords)
{
	for (int i = 0; i < coords.size(); i++)
	{
		coords[i].x -= sysInfo.dimensions.x / 2;
		coords[i].y -= sysInfo.dimensions.y / 2;
		coords[i].z -= sysInfo.dimensions.z / 2;
	}
}

std::vector<glm::vec3> DataInput::loadCoords()
{
	std::vector <int> mat;
	std::vector <int> cat;
	std::vector <std::string> type;
	std::vector <std::string> filenames;

	std::vector<glm::vec3> coords;

	std::ifstream coord_file;
	std::string line;
	std::stringstream ss;

	// open coordinate file
	ss << folder << "atoms-coords.cfg";
	
	coord_file.open((ss.str()).c_str());
	if (coord_file.fail())
	{
		std::cerr << "Error: Opening " << (ss.str()).c_str() << " failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "Reading file: " << folder << "atoms-coords.cfg" << std::endl;

	//-------------------------------------------
	//			File Header
	//-------------------------------------------
	getline(coord_file, line); // line
	getline(coord_file, line); // Title
	getline(coord_file, line); // line
	getline(coord_file, line); // Date
	getline(coord_file, line); // line
	getline(coord_file, line); // number of atoms
	getline(coord_file, line); // line
	unsigned int n_files;
	getline(coord_file, line); // number of subsidiary files
	line.erase(line.begin(), line.begin() + 22);
	n_files = atoi(line.c_str());
	for (unsigned int i = 0; i < n_files; i++) {
		getline(coord_file, line); // subsidiary files
		std::stringstream ss;
		ss << folder << line;
		filenames.push_back(ss.str());
	}
	getline(coord_file, line); // line
							   //-------------------------------------------
							   //
							   //-------------------------------------------
							   // resize arrays
	mat.resize(sysInfo.numAtoms);
	cat.resize(sysInfo.numAtoms);
	coords.resize(sysInfo.numAtoms);
	type.resize(sysInfo.numAtoms);


	unsigned int n_local_atoms;
	getline(coord_file, line); // number of local atoms
	n_local_atoms = atoi(line.c_str());

	unsigned int counter = 0;
	// read in coords
	for (unsigned int i = 0; i < n_local_atoms; i++) {
		coord_file >> mat[counter] >> cat[counter] >> coords[counter].x >> coords[counter].y >> coords[counter].z >> type[counter];
		counter++;
	}

	// close master file
	coord_file.close();

	// now read subsidiary files
	for (unsigned int file = 0; file < n_files; file++) {
		std::ifstream infile;
		infile.open(filenames[file].c_str());

		getline(infile, line);// number of local atoms
		n_local_atoms = atoi(line.c_str());
		// read in coords
		for (unsigned int i = 0; i < n_local_atoms; i++) {
			infile >> mat[counter] >> cat[counter] >> coords[counter].x >> coords[counter].y >> coords[counter].z >> type[counter];
			counter++;
		}
		// close subsidiary file
		infile.close();
	}

	//  check if read in correct number of coordinates
	if (counter != sysInfo.numAtoms) std::cerr << "Error in reading in coordinates" << std::endl;

	//Center System
	centerCoords(coords);

	// Create material index
	indexMat.resize(sysInfo.numMaterials);
	for (int i = 0; i < coords.size(); i++)
	{
		indexMat[mat[i]].push_back(i);
	}

	return coords;
}

std::vector<glm::vec3> DataInput::loadSpins(int spinFileNum)
{
	std::vector <std::string> filenames(0);
	std::string line;

	std::vector<glm::vec3> spins;

	std::stringstream file_sstr;
	file_sstr << folder << "atoms-";
	file_sstr << std::setfill('0') << std::setw(8) << (initSpin + spinFileNum);
	file_sstr << ".cfg";
	std::string cfg_file = file_sstr.str();
	int counter = 0;

	std::ifstream spinfile;
	spinfile.open(cfg_file.c_str());
	if (spinfile.fail())
	{
		std::cerr << "Error: Opening " << cfg_file.c_str() << " failed" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "Reading file: " << cfg_file << std::endl;

	//-------------------------------------------
	//			File Header
	//-------------------------------------------
	getline(spinfile, line); // line
	getline(spinfile, line); // name
	getline(spinfile, line); // line
	getline(spinfile, line); // date
	getline(spinfile, line); // line
	getline(spinfile, line); // number of atoms
	getline(spinfile, line); // sys dimensions
	getline(spinfile, line); // coord file
	getline(spinfile, line); // time
	getline(spinfile, line); // field
	getline(spinfile, line); // temp
	getline(spinfile, line); // Magnetisation
	getline(spinfile, line); // Number of materials
	for (int i = 0; i < sysInfo.numMaterials; i++) {
		getline(spinfile, line); // materials
	}
	getline(spinfile, line); // line
	unsigned int n_files;
	getline(spinfile, line); // number of subsidiary files
	line.erase(line.begin(), line.begin() + 22);
	n_files = atoi(line.c_str());
	for (unsigned int i = 0; i < n_files; i++) {
		getline(spinfile, line); // subsidiary file
		std::stringstream ss;
		ss << folder << line;
		filenames.push_back(ss.str());
	}
	getline(spinfile, line); // line
	//-------------------------------------------
	//
	//-------------------------------------------

	spins.resize(sysInfo.numAtoms); //Resize array

	unsigned int n_local_atoms;
	getline(spinfile, line); // number of local atoms
	n_local_atoms = atoi(line.c_str());

	// Read in spin coordinates
	for (unsigned int i = 0; i < n_local_atoms; i++) {
		spinfile >> spins[counter].x >> spins[counter].y >> spins[counter].z;
		counter++;
	}
	spinfile.close();// close master file

					 // now read subsidiary files
	for (unsigned int file = 0; file < n_files; file++) {
		std::ifstream infile;
		infile.open(filenames[file].c_str());

		getline(infile, line);// number of local atoms
		n_local_atoms = atoi(line.c_str());
		// Read in spin coordinates
		for (unsigned int i = 0; i < n_local_atoms; i++) {
			infile >> spins[counter].x >> spins[counter].y >> spins[counter].z;
			counter++;
		}
		infile.close();
	}

	// check if read in correct number of spins
	if (counter != sysInfo.numAtoms) std::cerr << "Error in reading in spins" << std::endl;

	return spins;
}

std::vector <std::vector<glm::vec3>> DataInput::splitData(std::vector<glm::vec3> Data)
{
	std::vector <std::vector<glm::vec3>> matData;
	matData.resize(sysInfo.numMaterials);
	for (int i = 0; i < indexMat.size(); i++)
	{
		for (int j = 0; j < indexMat[i].size(); j++)
		{
			matData[i].push_back(Data[indexMat[i][j]]);
		}
	}
	return matData;
}

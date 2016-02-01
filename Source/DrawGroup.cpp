#include "DrawGroup.h"
#include <map>
//#include "GLError.h"

void indexVBO(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec3> & in_normals,

	std::vector<int> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec3> & out_normals
	);

DrawGroup::DrawGroup()
{
}

void DrawGroup::setCoords(std::vector<glm::vec3> in_Coords)
{
	Coords.swap(in_Coords);
}

void DrawGroup::addSpins(std::vector<glm::vec3> Spins)
{
	AllSpins.push_back(Spins);

	CurrentSpins = AllSpins.begin();
	//(*CurrentSpins)

	for (int i = 0; i < (*CurrentSpins).size(); i++)
	{
		float theta = acos((*CurrentSpins)[i].z);
		float phi = atan((*CurrentSpins)[i].y, (*CurrentSpins)[i].x);
		//std::cout << (*CurrentSpins)[i].x << ", " << (*CurrentSpins)[i].y << ", " << (*CurrentSpins)[i].z << "\n Theta: " << theta << "\n Phi: " << phi << std::endl;
	}

}

void DrawGroup::OBJloader(std::string filename)
{

	std::ifstream objfile(filename, std::ifstream::in);
	if (!objfile)
	{
		std::cerr << "Cannot open " << filename << std::endl; exit(1);
	}

	std::vector<glm::vec3> inVertices, inNormals;

	Vertices.resize(0);
	Normals.resize(0);
	Indices.resize(0);

	std::vector<glm::vec3> tempVertices, tempNormals;
	std::vector<int> tempIndices;

	std::vector<int> normalIndices(0);

	std::string line;
	while (getline(objfile, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			std::istringstream s(line.substr(2));
			glm::vec3 v; s >> v.x; s >> v.y; s >> v.z;
			tempVertices.push_back(v);
		}
		else if (line.substr(0, 3) == "vn ")
		{
			std::istringstream s(line.substr(2));
			glm::vec3 v; s >> v.x; s >> v.y; s >> v.z;
			tempNormals.push_back(v);
		}
		else if (line.substr(0, 2) == "f ")
		{
			std::string a;
			for (std::istringstream s(line.substr(2)); s >> a; )
			{
				tempIndices.push_back(atoi(a.substr(0, a.find("//")).c_str()) - 1);
				normalIndices.push_back(atoi(a.substr(a.find("//") + 2).c_str()) - 1);
			}
		}
		else if (line[0] == '#')
		{
			/* ignoring this line */
		}
		else
		{
			/* ignoring this line */
		}
	}

	for (int i = 0; i < tempIndices.size(); i++)
	{
		inVertices.push_back(tempVertices[tempIndices  [i]]);
		inNormals.push_back (tempNormals [normalIndices[i]]);
	}

	indexVBO(inVertices, inNormals, Indices, Vertices, Normals);

	VerticesSize = Vertices.size();
	NormalsSize = Normals.size();
	IndicesSize = Indices.size();
	CoordsSize = Coords.size();
}

void DrawGroup::swapSpins(std::vector<glm::vec3> &newSpins)
{
	Spins.swap(newSpins);
}

void DrawGroup::loadBuffers()
{
	glewExperimental = GL_TRUE;
	glewInit();
	
	//  Create VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//-------------------------------------------------------------
	GLint components;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	const GLvoid* pointer=(void*)0;
	//-------------------------------------------------------------
	
	// Vertex Buffer
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(glm::vec3), &Vertices[0], GL_DYNAMIC_DRAW); // &____[0], ____.data() or &____.front()
	
	//  Pass vertex data
	GLint vertexAttrib = 0;
	glEnableVertexAttribArray(vertexAttrib);
	components = 3;
	type = GL_FLOAT;
	normalized = GL_FALSE;
	stride = 0; //Stride zero = tightly packed data
	pointer = (void*)0;
	

	glVertexAttribPointer(vertexAttrib, components, type, normalized, stride, 0);

	glVertexAttribDivisor(vertexAttrib, 0); // Vetecies: Reuse same
	//-------------------------------------------------------------
	
	// Normal Buffer
	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(glm::vec3), &Normals[0], GL_DYNAMIC_DRAW);

	//  Pass normal data
	GLint normalAttrib = 1;
	glEnableVertexAttribArray(normalAttrib);
	components = 3;
	type = GL_FLOAT;
	normalized = GL_FALSE;
	stride = 0;
	pointer = (void*)0;
	glVertexAttribPointer(normalAttrib, components, type, normalized, stride, pointer);
	glVertexAttribDivisor(normalAttrib, 0); // Normals: Reuse same
	//-------------------------------------------------------------
	
	// Index Buffer	
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(int), &Indices[0], GL_DYNAMIC_DRAW);

	//-------------------------------------------------------------
	
	// Coordinate Buffer
	glGenBuffers(1, &coordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, coordBuffer);
	glBufferData(GL_ARRAY_BUFFER, Coords.size() * sizeof(glm::vec3), &Coords[0], GL_DYNAMIC_DRAW);

	//  Pass coord data
	GLint corAttrib = 2;
	glEnableVertexAttribArray(corAttrib);
	components = 3;
	type = GL_FLOAT;
	normalized = GL_FALSE;
	stride = 0;
	pointer = (void*)0;
	glVertexAttribPointer(corAttrib, components, type, normalized, stride, pointer);
	glVertexAttribDivisor(corAttrib, 1); // Coord: 1 per atom
	//-------------------------------------------------------------
	
	// Spin Buffer
	CurrentSpins = AllSpins.begin();
	glGenBuffers(1, &spinBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, spinBuffer);
	glBufferData(GL_ARRAY_BUFFER, (*CurrentSpins).size() * sizeof(glm::vec3), &(*CurrentSpins)[0], GL_DYNAMIC_DRAW);

	//  Pass spin data
	GLint spiAttrib = 3;
	glEnableVertexAttribArray(spiAttrib);
	components = 3;
	type = GL_FLOAT;
	normalized = GL_FALSE;
	stride = 0;
	pointer = (void*)0;
	glVertexAttribPointer(spiAttrib, components, type, normalized, stride, pointer);
	glVertexAttribDivisor(spiAttrib, 1); // Spin: 1 per atom 
//-------------------------------------------------------------
}

void DrawGroup::genPyramind()
{
	Vertices.resize(0);
	Vertices = { glm::vec3(0.0, 0.0, 1.0),
		vec3(0.5, 0.5, -0.5),
		vec3(0.5, -0.5, -0.5),
		vec3(-0.5, 0.5, -0.5),
		vec3(-0.5, -0.5, -0.5) };

	Indices.resize(0);
	Indices = {
		2, 0, 1,
		4, 0, 2,
		3, 0, 4,
		1, 0, 3,
		4, 2, 1,
		1, 3, 4 };
	Normals.resize(0);
	for (unsigned int i = 0; i < Vertices.size(); i++)
	{
		Normals.push_back(glm::vec3(0.0, 0.0, 1.0));
	}
}

void DrawGroup::genIsosphere()
{
	const float t = 1.6180339887498;//(1.0 +Sqrt(5.0)) / 2.0
	Vertices.resize(0);
	Vertices = {
		vec3(-1.0, t, 0.0),
		vec3(1.0, t, 0.0),
		vec3(-1.0, -t, 0.0),
		vec3(1.0, -t, 0.0),

		vec3(0.0, -1.0, t),
		vec3(0.0, 1.0, t),
		vec3(0.0, -1.0, -t),
		vec3(0.0, 1.0, -t),

		vec3(t, 0.0, -1.0),
		vec3(t, 0.0, 1.0),
		vec3(-t, 0.0, -1.0),
		vec3(-t, 0.0, 1.0) };

	Indices.resize(0);
	Indices = {
		0, 11, 5,// 5 faces around point 0
		0, 5, 1,
		0, 1, 7,
		0, 7, 10,
		0, 10, 11,

		1, 5, 9,// 5 adjacent faces
		5, 11, 4,
		11, 10, 2,
		10, 7, 6,
		7, 1, 8,

		3, 9, 4,// 5 faces around point 3
		3, 4, 2,
		3, 2, 6,
		3, 6, 8,
		3, 8, 9,

		4, 9, 5,// 5 adjacent faces
		2, 4, 11,
		6, 2, 10,
		8, 6, 7,
		9, 8, 1 };
	Normals.resize(0);
	for (unsigned int i = 0; i < Vertices.size(); i++)
	{
		Normals.push_back(glm::vec3(0.0, 0.0, 1.0));
	}
}

void DrawGroup::Buffers()
{	//  Create VAO

	const unsigned int SizeOfVec3 = sizeof(glm::vec3);
	
	glBindVertexArray(vao);
	//-------------------------------------------------------------
	GLint components;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	void* pointer;
	//-------------------------------------------------------------
	// Vertex Buffer
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, VerticesSize * SizeOfVec3, &Vertices[0], GL_DYNAMIC_DRAW); // &____[0], ____.data() or &____.front()
																										   //  Pass vertex data
	GLint vertexAttrib = 0;
	glEnableVertexAttribArray(vertexAttrib);
	components = 3;
	type = GL_FLOAT;
	normalized = GL_FALSE;
	stride = 0; //Stride zero = tightly packed data
	pointer = (void*)0;
	glVertexAttribPointer(vertexAttrib, components, type, normalized, stride, pointer);
	glVertexAttribDivisor(vertexAttrib, 0); // Vetecies: Reuse same
											//-------------------------------------------------------------
											// Normal Buffer
	
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, NormalsSize * SizeOfVec3, &Normals[0], GL_DYNAMIC_DRAW);

	//  Pass normal data
	GLint normalAttrib = 1;
	glEnableVertexAttribArray(normalAttrib);
	components = 3;
	type = GL_FLOAT;
	normalized = GL_FALSE;
	stride = 0;
	pointer = (void*)0;
	glVertexAttribPointer(normalAttrib, components, type, normalized, stride, pointer);
	glVertexAttribDivisor(normalAttrib, 0); // Normals: Reuse same
											//-------------------------------------------------------------
											// Index Buffer	
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(int), &Indices[0], GL_DYNAMIC_DRAW);

	//-------------------------------------------------------------
	// Coordinate Buffer
	glBindBuffer(GL_ARRAY_BUFFER, coordBuffer);
	glBufferData(GL_ARRAY_BUFFER, CoordsSize * SizeOfVec3, &Coords[0], GL_DYNAMIC_DRAW);

	//  Pass coord data
	GLint corAttrib = 2;
	glEnableVertexAttribArray(corAttrib);
	components = 3;
	type = GL_FLOAT;
	normalized = GL_FALSE;
	stride = 0;
	pointer = (void*)0;
	glVertexAttribPointer(corAttrib, components, type, normalized, stride, pointer);
	glVertexAttribDivisor(corAttrib, 1); // Coord: 1 per atom
										 //-------------------------------------------------------------
										 // Spin Buffer
	
	glBindBuffer(GL_ARRAY_BUFFER, spinBuffer);
	glBufferData(GL_ARRAY_BUFFER, (*CurrentSpins).size() * SizeOfVec3, &(*CurrentSpins)[0], GL_DYNAMIC_DRAW);

	//  Pass spin data
	GLint spiAttrib = 3;
	glEnableVertexAttribArray(spiAttrib);
	components = 3;
	type = GL_FLOAT;
	normalized = GL_FALSE;
	stride = 0;
	pointer = (void*)0;
	glVertexAttribPointer(spiAttrib, components, type, normalized, stride, pointer);
	glVertexAttribDivisor(spiAttrib, 1); // Spin: 1 per atom 								 //-------------------------------------------------------------
}

void DrawGroup::updateSpinBufffer()
{
	GLint components;
	GLenum type;
	GLboolean normalized;
	GLsizei stride;
	void* pointer;

	//GLuint spinBuffer;
	//glGenBuffers(1, &spinBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, spinBuffer);
	glBufferData(GL_ARRAY_BUFFER, (*CurrentSpins).size() * sizeof(glm::vec3), &(*CurrentSpins)[0], GL_DYNAMIC_DRAW);

	//  Pass spin data
	GLint spiAttrib = 3;
	glEnableVertexAttribArray(spiAttrib);
	components = 3;
	type = GL_FLOAT;
	normalized = GL_FALSE;
	stride = 0;
	pointer = (void*)0;
	glVertexAttribPointer(spiAttrib, components, type, normalized, stride, pointer);
	glVertexAttribDivisor(spiAttrib, 1); // Spin: 1 per atom 
}

void DrawGroup::update()
{
	if (show)
	{
		//load Buffers
		Buffers();
		// Draw DrawGroups
		glDrawElementsInstanced(GL_TRIANGLES, IndicesSize, GL_UNSIGNED_INT, (void*)0, CoordsSize);

	}
}

DrawGroup::~DrawGroup()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &coordBuffer);
	glDeleteBuffers(1, &spinBuffer);
}

void DrawGroup::SpinstepAdd()
{
	CurrentSpins++;
	if (CurrentSpins == AllSpins.end())
	{
		CurrentSpins--;
		std::cout << "end" << std::endl;
	}

	glBindBuffer(GL_ARRAY_BUFFER, spinBuffer);
	glBufferData(GL_ARRAY_BUFFER, (*CurrentSpins).size() * sizeof(glm::vec3), &(*CurrentSpins)[0], GL_DYNAMIC_DRAW);
	return;
}

void DrawGroup::SpinstepMinus()
{
	if (CurrentSpins == AllSpins.begin())
	{
		std::cout << "begin" << std::endl;
		return;
	}
	CurrentSpins--;

	glBindBuffer(GL_ARRAY_BUFFER, spinBuffer);
	glBufferData(GL_ARRAY_BUFFER, (*CurrentSpins).size() * sizeof(glm::vec3), &(*CurrentSpins)[0], GL_DYNAMIC_DRAW);
	return;
}

void DrawGroup::visble()
{
	if (show){ show = false;}
	else
	{show = true;}
}

//----------------------------------------------
// STOLEN FROM opengl-tutorial.org
//----------------------------------------------

struct PackedVertex {
	glm::vec3 position;
	glm::vec3 normal;
	bool operator<(const PackedVertex that) const {
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
	};
};

bool getSimilarVertexIndex_fast(
	PackedVertex & packed,
	std::map<PackedVertex, unsigned short> & VertexToOutIndex,
	unsigned short & result
	) {
	std::map<PackedVertex, unsigned short>::iterator it = VertexToOutIndex.find(packed);
	if (it == VertexToOutIndex.end()) {
		return false;
	}
	else {
		result = it->second;
		return true;
	}
}

void indexVBO(
	std::vector<glm::vec3> & in_vertices,
	std::vector<glm::vec3> & in_normals,

	std::vector<int> & out_indices,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec3> & out_normals
	) {
	std::map<PackedVertex, unsigned short> VertexToOutIndex;

	// For each input vertex
	for (unsigned int i = 0; i<in_vertices.size(); i++) {

		PackedVertex packed = { in_vertices[i], in_normals[i] };


		// Try to find a similar vertex in out_XXXX
		unsigned short index;
		bool found = getSimilarVertexIndex_fast(packed, VertexToOutIndex, index);

		if (found) { // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back(index);
		}
		else { // If not, it needs to be added in the output data.
			out_vertices.push_back(in_vertices[i]);
			out_normals.push_back(in_normals[i]);
			int newindex = (int)out_vertices.size() - 1;
			out_indices.push_back(newindex);
			VertexToOutIndex[packed] = newindex;
		}
	}
}

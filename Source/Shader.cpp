#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>

GLuint ProgramID;

GLuint getShader() { return ProgramID; }

//LoadShaders(vertex_file_path, fragment_file_path, geometry_file_path) Loads and opens all three shaders
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path, const char * geometry_file_path)
{
	//	Create program ID and Shader ID
	ProgramID = glCreateProgram();
	GLuint VertexShaderID = CreateShader(ReadShader(vertex_file_path), GL_VERTEX_SHADER);
	GLuint GeometryShaderID = CreateShader(ReadShader(geometry_file_path), GL_GEOMETRY_SHADER);
	GLuint FragmentShaderID = CreateShader(ReadShader(fragment_file_path), GL_FRAGMENT_SHADER);

	//	Link Shaders to Program
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, GeometryShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);
	glUseProgram(ProgramID);

	//	Check if linked corectly
	CheckShaderError(ProgramID, GL_LINK_STATUS, true, "Error: Program linking failed");

	//	Validate if shaders/progra is working
	glValidateProgram(ProgramID);
	CheckShaderError(ProgramID, GL_VALIDATE_STATUS, true, "Error: Program Validate failed");

	//	Return program ID to be used in code
	return ProgramID;
}


GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path)
{
	//	Create program ID and Shader ID
	ProgramID = glCreateProgram();
	GLuint VertexShaderID = CreateShader(ReadShader(vertex_file_path), GL_VERTEX_SHADER);
	GLuint FragmentShaderID = CreateShader(ReadShader(fragment_file_path), GL_FRAGMENT_SHADER);

	//	Link Shaders to Program
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);
	glUseProgram(ProgramID);

	//	Check if linked corectly
	CheckShaderError(ProgramID, GL_LINK_STATUS, true, "Error: Program linking failed");

	//	Validate if shaders/progra is working
	glValidateProgram(ProgramID);
	CheckShaderError(ProgramID, GL_VALIDATE_STATUS, true, "Error: Program Validate failed");

	//	Return program ID to be used in code
	return ProgramID;
}

static GLuint CreateShader(const std::string& text, GLenum shaderType)
{
	//	Create shader and check it
	GLuint shaderID = glCreateShader(shaderType);
	if (shaderID == 0)
		std::cerr << "Error: Shader creation failed!" << std::endl;

	//	Convert source string to GLchar*
	const GLchar* shaderSourceStrings[1];
	shaderSourceStrings[0] = text.c_str();
	//	Get Length
	GLint shaderSourceStringLengths[1];
	shaderSourceStringLengths[0] = text.length();

	//	Compile Shader and check it
	glShaderSource(shaderID, 1, shaderSourceStrings, shaderSourceStringLengths);
	glCompileShader(shaderID);
	CheckShaderError(shaderID, GL_COMPILE_STATUS, false, "Error: Shader comilation failed");

	// Return Shader ID
	return shaderID;
}

static std::string ReadShader(const std::string& fileName)
{
	std::ifstream file;
	std::string output;
	std::string line;

	// Open and read file
	file.open((fileName).c_str());
	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
		std::cerr << "Unable to load shader: " << fileName << std::endl;

	// Return contents of file as string
	return output;
}

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	//Find infomation for program or shader
	if (isProgram)
		glGetProgramiv(shader, flag, &success);
	else
		glGetShaderiv(shader, flag, &success);

	//If there is an error display error message
	if (success == GL_FALSE)
	{
		if (isProgram)
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		else
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);

		std::cerr << errorMessage << ": '" << error << "'" << std::endl;
	}
}
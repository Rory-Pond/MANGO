#pragma once

#include <GL/glew.h>
#include <string>

//LoadShaders(vertex_file_path, fragment_file_path, geometry_file_path) 
//Loads and opens all three shaders
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path, const char * geometry_file_path);

//LoadShaders(vertex_file_path, fragment_file_path) Loads and opens all two stadard shaders
GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

//Reads in shader from file and returns as a string
static std::string ReadShader(const std::string& fileName);

//Creates OpenGL shader from string
static GLuint CreateShader(const std::string& text, GLenum shaderType);

//Checks for shader errors
static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);

//Returns ProgramID
GLuint getShader();

std::string InstancedVertexShader();
std::string InstancedFragmentShader();



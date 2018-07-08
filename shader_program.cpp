#include "shader_program.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

// default constructor
ShaderProgram::ShaderProgram()
{
}

// destructor, deallocate OpenGL program resources
ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(program);
}

// compiles and links a program from specified shader files
void ShaderProgram::loadProgram(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath)
{
	// compile shaders from file paths
	GLuint vertexShader{ loadShaderFile(GL_VERTEX_SHADER, vertexPath) };
	GLuint fragmentShader{ loadShaderFile(GL_FRAGMENT_SHADER, fragmentPath) };
	GLuint geometryShader{ geometryPath ? loadShaderFile(GL_GEOMETRY_SHADER, geometryPath) : 0 };

	// create a program and attach shaders
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	if (geometryPath)
	{
		glAttachShader(program, geometryShader);
	}

	// query for link status
	glLinkProgram(program);
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus)
	{
		// retrieve info log information
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog{ new GLchar[infoLogLength + 1] };
		glGetProgramInfoLog(program, infoLogLength, nullptr, infoLog);

		// print error message
		std::cerr << "Failed to link shader program: " << infoLog;
		delete[] infoLog;
	}

	// detach and delete shaders
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	if (geometryPath)
	{
		glDetachShader(program, geometryShader);
		glDeleteShader(geometryShader);
	}
}

// use program
void ShaderProgram::use()
{
	glUseProgram(program);
}

// creates a shader from C-style string source
GLuint ShaderProgram::createShader(GLenum shaderType, const GLchar* shaderSource)
{
	// create and compile shader
	GLuint shader{ glCreateShader(shaderType) };
	glShaderSource(shader, 1, &shaderSource, nullptr);
	glCompileShader(shader);

	// query for compile status
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (!compileStatus)
	{
		// retrieve info log information
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog{ new GLchar[infoLogLength + 1] };
		glGetShaderInfoLog(shader, infoLogLength, nullptr, infoLog);

		// print error message
		const char* strShaderType{ nullptr };
		switch (shaderType)
		{
		case GL_VERTEX_SHADER:
			strShaderType = "vertex";
			break;
		case GL_FRAGMENT_SHADER:
			strShaderType = "fragment";
			break;
		case GL_GEOMETRY_SHADER:
			strShaderType = "geometry";
			break;
		default:
			strShaderType = "unknown";
			break;
		}

		std::cerr << "Failed to compile " << strShaderType << " shader: " << infoLog;
		delete[] infoLog;
	}

	return shader;
}

// returns a shader created from a file path
GLuint ShaderProgram::loadShaderFile(GLenum shaderType, const char* path)
{
	std::ifstream shaderFile(path);
	std::stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();

	std::string shaderString(shaderStream.str());
	const GLchar* shaderSource{ shaderString.c_str() };
	return createShader(shaderType, shaderSource);
}
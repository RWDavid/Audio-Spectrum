#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

class ShaderProgram
{
public:
	// default constructor
	ShaderProgram();

	// destructor
	~ShaderProgram();

	// compiles and links program from specified shader files
	void loadProgram(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = nullptr);

	// use program
	void use();

	// OpenGL program ID
	GLuint program;

private:
	GLuint createShader(GLenum shaderType, const GLchar* shaderSource);		// creates a shader from C-style string source
	GLuint loadShaderFile(GLenum shaderType, const char* path);				// returns a shader created from a file path
};

#endif
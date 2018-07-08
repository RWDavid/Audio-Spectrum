#ifndef BAR_H
#define BAR_H

#include <glad/glad.h>
#include <array>
#include "shader_program.h"

class Bar
{
public:
	// default constructor
	Bar();

	// constructor, assign vertex positions with supplied 
	// arguments and set up OpenGL vertex and index buffers
	Bar(GLfloat width, GLfloat height, GLfloat xPos, GLfloat yPos);

	void setHeight(GLfloat height); // set bar height
	void update();					// update vertex positions based on width/height/position
	void draw();					// render bars

	static void setProgram(ShaderProgram &program);		// set OpenGL shader program to be used when rendering

private:
	GLfloat width;
	GLfloat height;
	GLfloat xPos;
	GLfloat yPos;

	GLfloat vertexWidth;
	GLfloat vertexHeight;
	GLfloat vertexPosX;
	GLfloat vertexPosY;

	GLuint VAO;
	GLuint VBO;
	GLuint EBO;

	static ShaderProgram s_program;

	std::array<GLfloat, 8> vertexData;
	std::array<GLuint, 6> indexData;
};

#endif
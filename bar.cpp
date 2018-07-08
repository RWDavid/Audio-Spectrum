#include "bar.h"
#include "window.h"

// static member variable declarations
ShaderProgram Bar::s_program;

// default constructor, set up OpenGL vertex and index buffers
Bar::Bar()
{
	// indices to draw bar
	indexData =
	{
		0, 1, 3,
		2, 3, 1
	};

	// set up vertex array object, vertex buffer object, and element buffer object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData.data(), GL_STREAM_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// constructor, assign vertex positions with supplied 
// arguments and set up OpenGL vertex and index buffers
Bar::Bar(GLfloat width, GLfloat height, GLfloat xPos, GLfloat yPos)
	: width{ width }, height{ height }, xPos{ xPos }, yPos{ yPos }
{
	// indices to draw bar
	indexData =
	{
		0, 1, 3,
		2, 3, 1
	};

	// set up vertex array object, vertex buffer object, and element buffer object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData.data(), GL_STREAM_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)(0));
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexData), indexData.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// assign vertex positions and send to vertex buffer object
	update();
}

// set bar height
void Bar::setHeight(GLfloat height)
{
	this->height = height;
}

// set OpenGL shader program to be used when rendering
void Bar::setProgram(ShaderProgram &program)
{
	s_program = program;
}

// update vertex positions based on width/height/position
void Bar::update()
{
	vertexWidth = width / Window::width * 2;
	vertexHeight = height / Window::height * 2;
	vertexPosX = xPos / Window::width * 2 - 1;
	vertexPosY = yPos / Window::height * 2 - 1;

	vertexData =
	{
		vertexPosX, vertexPosY,									// bottom left
		vertexPosX, vertexPosY + vertexHeight,					// top left
		vertexPosX + vertexWidth, vertexPosY + vertexHeight,	// top right
		vertexPosX + vertexWidth, vertexPosY					// bottom right
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexData), vertexData.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// render bars
void Bar::draw()
{
	s_program.use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)(0));
	glBindVertexArray(0);
}
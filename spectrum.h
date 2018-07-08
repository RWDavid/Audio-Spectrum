#ifndef SPECTRUM_H
#define SPECTRUM_H

#include <glad/glad.h>
#include <vector>
#include "bar.h"

class Spectrum
{
public:
	Spectrum(int length, GLfloat spacing);

	void update();
	void draw();
	std::vector<Bar> bars;

private:
	const int length;
};

#endif
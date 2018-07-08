#include "spectrum.h"
#include "window.h"

Spectrum::Spectrum(int length, GLfloat spacing) : length{ length }
{
	GLfloat freqRes = static_cast<GLfloat>(44100.0 / (length * 2));
	std::vector<GLfloat> positions(length - 1, 0.0f);
	std::vector<GLfloat> barWidths(length - 1, 0.0f);

	for (int bin{ 1 }; bin < length; ++bin)
	{
		positions[bin - 1] = log10f(static_cast<GLfloat>(bin));
	}

	GLfloat scale{ (Window::width - 150.0f) / positions[positions.size() - 1] };

	for (unsigned int x{ 0 }; x < positions.size(); ++x)
	{
		positions[x] = positions[x] * scale + 75.0f;
	}

	for (unsigned int x{ 0 }; x < barWidths.size() - 1; ++x)
	{
		barWidths[x] = positions[x + 1] - positions[x];
	}
	barWidths[barWidths.size() - 1] = barWidths[barWidths.size() - 2];

	const GLfloat step{ Window::width / static_cast<GLfloat>(length) };
	for (int count{ 0 }; count < length - 1; ++count)
	{
		bars.push_back(Bar(barWidths[count], 300.0f, positions[count], 100.0f));
	}
}

void Spectrum::update()
{
	for (unsigned int count{ 0 }; count < bars.size(); ++count)
	{
		bars[count].update();
	}
}

void Spectrum::draw()
{
	for (unsigned int count{ 0 }; count < bars.size(); ++count)
	{
		bars[count].draw();
	}
}
#include <iostream>
#include <algorithm>
#include <string>
#include <glad/glad.h>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include "shader_program.h"
#include "window.h"
#include "spectrum.h"
#include "sound.h"

int main()
{
	// opengl ver 3.3 core profile
	sf::ContextSettings contextSettings;
	contextSettings.majorVersion = 3;
	contextSettings.minorVersion = 3;
	contextSettings.attributeFlags = sf::ContextSettings::Core;

	// create window
	sf::Window window(sf::VideoMode(Window::width, Window::height), "LearnOpenGL", sf::Style::Default, contextSettings);

	// initialize GLAD
	if (!gladLoadGL()) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// set bar rendering shaders
	ShaderProgram program;
	program.loadProgram("shaders/basic.vert", "shaders/basic.frag");
	Bar::setProgram(program);

	// initialize spectrum of bars
	Spectrum spectrum(4096, 0.0f);

	std::cout << "FREQUENCY SPECTRUM VISUALIZER\n\n";
	std::cout << "Controls\n--------\n";
	std::cout << "Space\t\tToggle Audio Playback\n";
	std::cout << "Up\t\tIncrease Bar Height\n";
	std::cout << "Down\t\tDecrease Bar Height\n";
	std::cout << "Esc\t\tClose Window\n\n";

	std::cout << "Specify audio path (mp3 not supported): ";
	std::string audioPath;
	std::getline(std::cin, audioPath);

	// load sound file and play
	Sound mySound(audioPath, 8192);
	mySound.play();

	bool windowIsOpen{ true };
	float intensity = 0.20f;
	while (windowIsOpen)
	{
		window.setActive();
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					mySound.toggle();
				}

				if (event.key.code == sf::Keyboard::Escape)
				{
					windowIsOpen = false;
				}

				if (event.key.code == sf::Keyboard::Up)
				{
					if (intensity < 1.0f)
					{
						intensity += .01f;
					}
				}

				if (event.key.code == sf::Keyboard::Down)
				{
					if (intensity > .009f)
					{
						intensity -= .01f;
					}
				}
			}
			if (event.type == sf::Event::Closed)
			{
				windowIsOpen = false;
			}
			else if (event.type == sf::Event::Resized)
			{
				int ratioWidth{ 1 };
				int ratioHeight{ 1 };

				GLuint width, height;
				if (event.size.width * ratioHeight > event.size.height * ratioWidth)
				{
					height = event.size.height;
					width = height * ratioWidth / ratioHeight;
					glViewport((event.size.width - width) / 2, 0, width, height);
				}
				else
				{
					width = event.size.width;
					height = width * ratioHeight / ratioWidth;
					glViewport(0, (event.size.height - height) / 2, width, height);
				}
			}
		}

		mySound.update();
		for (int bin{ 1 }; bin < 4096; ++bin)
		{
			GLfloat value = static_cast<GLfloat>(abs(mySound.fftBins[bin])) * intensity; // arbitrary scaling value
			spectrum.bars[bin - 1].setHeight(std::min(value, 600.0f));
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		spectrum.update();
		spectrum.draw();

		window.display();
	}

	return 0;
}
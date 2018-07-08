#define _USE_MATH_DEFINES

#include "sound.h"

#include <iostream>
#include <cmath>
#include "fft.h"

Sound::Sound(const std::string &soundPath, int fftSize)
{
	// attempt to load sound buffer from audio file
	if (!soundBuffer.loadFromFile(soundPath))
	{
		std::cerr << "Sound::Sound(): unable to load sound buffer\n";
	}
	sound.setBuffer(soundBuffer);
	sound.setLoop(true);

	// initialize member variables
	sampleRate = static_cast<int>(soundBuffer.getSampleRate());
	sampleCount = static_cast<int>(soundBuffer.getSampleCount());
	channelCount = static_cast<int>(soundBuffer.getChannelCount());
	samplePos = 0;
	this->fftSize = fftSize;
	freqRes = static_cast<double>(sampleRate) / fftSize;

	// initialize hann window
	for (int x{ 0 }; x < fftSize; ++x)
	{
		double y = sin((M_PI * x) / (fftSize - 1));
		hannWindow.push_back(y * y);
	}

	// initialize bin frequencies with freq resolution
	for (int x{ 0 }; x < fftSize; ++x)
	{
		binFreq.push_back(x * freqRes);
	}

	fftBins.resize(fftSize);
}

void Sound::update()
{
	// check for stereo input
	if (channelCount == 2)
	{
		// retrieve current sample data and apply hann window
		samplePos = static_cast<int>((sound.getPlayingOffset().asSeconds()) * (sampleRate * 2));
		if ((samplePos + fftSize * 2) >= sampleCount)
		{
			return;
		}
		for (int x{ samplePos }, y{ 0 }; x < (samplePos + fftSize * 2); x += 2, ++y)
		{
			// average stereo data: left and right samples are interleaved
			double sampleAverage{ (soundBuffer.getSamples()[x] + soundBuffer.getSamples()[x + 1]) / 2.0 };
			fftBins[y] = sampleAverage * hannWindow[y];
		}
	}
	// assuming mono input
	else
	{
		// retrieve current sample data and apply hann window
		samplePos = static_cast<int>((sound.getPlayingOffset().asSeconds()) * sampleRate);
		if ((samplePos + fftSize) >= sampleCount)
		{
			return;
		}
		for (int x{ samplePos }, y{ 0 }; x < (samplePos + fftSize); ++x, ++y)
		{
			fftBins[y] = soundBuffer.getSamples()[x] * hannWindow[y];
		}
	}
	// apply FFT
	FFT::forward(fftBins);
}

// play sound
void Sound::play()
{
	sound.play();
}

// pause sound
void Sound::pause()
{
	sound.pause();
}

// stop sound and reset playing position
void Sound::stop()
{
	sound.stop();
}

void Sound::toggle()
{
	if (sound.getStatus() == sf::Sound::Playing)
	{
		sound.pause();
	}

	else
	{
		sound.play();
	}
}

// set whether the sound should loop at the end
void Sound::setLoop(bool loop)
{
	sound.setLoop(loop);
}

// set volume of sound (0 through 100)
void Sound::setVolume(float volume)
{
	sound.setVolume(volume);
}

// retrieve looping status
bool Sound::getLoop()
{
	return sound.getLoop();
}

// retrieve sound volume (0 through 100)
float Sound::getVolume()
{
	return sound.getVolume();
}

// retrieve amount of seconds since the sound started
float Sound::getPlayingOffset()
{
	return sound.getPlayingOffset().asSeconds();
}

// retrieve sample rate of sound in Hz
int Sound::getSampleRate()
{
	return sampleRate;
}

// retrieve the amount of samples in the sound
int Sound::getSampleCount()
{
	return sampleCount;
}

// retrieve the amount of channels in the sound
int Sound::getChannelCount()
{
	return channelCount;
}

// retrieve the total duration of the sound in seconds
float Sound::getDuration()
{
	return soundBuffer.getDuration().asSeconds();
}

// retrieve the status of the sound (playing/paused/stopped)
sf::SoundSource::Status Sound::getStatus()
{
	return sound.getStatus();
}
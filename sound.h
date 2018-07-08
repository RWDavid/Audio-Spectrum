#ifndef SOUND_H
#define SOUND_H

#include <string>
#include <vector>
#include <complex>
#include <SFML/Audio.hpp>
#include "fft.h"

class Sound
{
public:
	// constructor
	Sound(const std::string &soundPath, int fftSize);

	void  update();					// update frequency bins
	void  play();					// play sound
	void  pause();					// pause sound
	void  stop();					// stop sound and reset playing position
	void  toggle();					// toggle playing / paused status
	void  setLoop(bool loop);		// set whether the sound should loop at the end
	void  setVolume(float volume);	// set volume of sound (0 through 100)
	bool  getLoop();				// retrieve looping status
	float getVolume();				// retrieve sound volume (0 through 100)
	float getPlayingOffset();		// retrieve amount of seconds since the sound started
	int   getSampleRate();			// retrieve sample rate of sound in Hz
	int   getSampleCount();			// retrieve the amount of samples in the sound
	int   getChannelCount();		// retrieve the amount of channels in the sound
	float getDuration();			// retrieve the total duration of the sound in seconds

									// retrieve the status of the sound (playing/paused/stopped)
	sf::SoundSource::Status getStatus();

	// complex vector of frequency bins
	std::vector<std::complex<double>> fftBins;

	// complex vector containing the frequencies of each frequency bin
	std::vector<double> binFreq;

private:
	int sampleRate;
	int sampleCount;
	int samplePos;
	int channelCount;
	int fftSize;
	double freqRes;

	// vector containing hann window multipliers
	std::vector<double> hannWindow;

	sf::SoundBuffer soundBuffer;
	sf::Sound sound;
};

#endif
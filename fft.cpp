#define _USE_MATH_DEFINES

#include "fft.h"

#include <iostream>
#include <vector>
#include <complex>
#include <utility>
#include <cmath>

namespace FFT
{
	/*
		Rearranges the elements of a complex vector with a bit 
		reversal algorithm. Length of vector must be a power of 2.

		dataIn - input vector to rearrange
		dataOut - output vector to hold the rearranged elements
		(length of dataIn must equal length of dataOut)

		Returns true on success and false on failure.
	*/
	bool reverseArrange(const std::vector<std::complex<double>> &dataIn, std::vector<std::complex<double>> &dataOut)
	{
		const unsigned int length{ static_cast<unsigned int>(dataIn.size()) };
		// input validation
		if ((length != dataOut.size()) || (length == 0) || (length & (length - 1)))
		{
			std::cerr << "FFT::reverseArrange(): invalid vector length\n";
			return false;
		}
		unsigned int swapIndex{ 0 };
		for (unsigned int currentIndex{ 0 }; currentIndex < length; ++currentIndex)
		{
			dataOut[swapIndex] = dataIn[currentIndex];
			unsigned int bitmask{ length };
			// determine the next element to swap according to
			// least significant bit order starting from the left 
			while (swapIndex & (bitmask >>= 1)) // if the bit is set
			{
				swapIndex &= ~bitmask; // clear bit and move on to the next bit
			}
			swapIndex |= bitmask; // current bit is cleared, so set the bit
		}
		return true;
	}

	/*
		Rearranges the elements of a complex vector in place with a
		bit reversal algorithm. Length of vector must be a power of 2.

		data - vector to rearrange

		Returns true on success and false on failure.
	*/
	bool reverseArrange(std::vector<std::complex<double>> &data)
	{
		const unsigned int length{ static_cast<unsigned int>(data.size()) };
		// input validation
		if ((length == 0) || (length & (length - 1)))
		{
			std::cerr << "FFT::reverseArrange(): invalid vector length\n";
			return false;
		}
		unsigned int swapIndex{ 0 };
		// swap every element in the array once
		for (unsigned int currentIndex{ 0 }; currentIndex < length; ++currentIndex)
		{
			if (swapIndex > currentIndex)
			{
				std::swap(data[currentIndex], data[swapIndex]);
			}
			unsigned int bitmask{ length };
			// determine the next element to swap according to
			// least significant bit order starting from the left 
			while (swapIndex & (bitmask >>= 1)) // if the bit is set
			{
				swapIndex &= ~bitmask; // clear bit and move on to the next bit
			}
			swapIndex |= bitmask; // current bit is cleared, so set the bit
		}
		return true;
	}

	/*
		Performs radix-2 Cooley-Tukey Fast Fourier Transform
		on a complex vector. Optional inverse parameter may
		be specified. Length of vector must be a power of 2.

		data - vector to transform

		Returns true on success and false on failure.
	*/
	bool transform(std::vector<std::complex<double>> &data, bool inverse)
	{
		const int length{ static_cast<int>(data.size()) };
		// input validation
		if ((length == 0) || (length & (length - 1)))
		{
			std::cerr << "FFT::transform(): invalid vector length\n";
			return false;
		}
		const double pi{ inverse ? -M_PI : M_PI };
		for (int step{ 1 }; step < length; step *= 2)
		{
			// jump to the next entry of the same transform factor
			const int jump{ step * 2 };
			const double delta{ -pi / step };
			const double sine{ sin(delta / 2) };
			// multiplier for transform factor
			const std::complex<double> multiplier{ -2 * sine * sine, sin(delta) };
			// start value for transform factor
			std::complex<double> factor{ 1.0 };
			// iteration through groups of different transform factor
			for (int group{ 0 }; group < step; ++group)
			{
				// calculate sums with the same transform factor
				for (int summand{ group }; summand < length; summand += jump)
				{
					// position of second term
					const int match{ summand + step };
					// second term of sum
					const std::complex<double> product{ factor * data[match] };
					// butterfly for in place vector processing
					data[match] = data[summand] - product;
					data[summand] += product;
				}
				// calculate next transform factor via trigonometric recurrence
				factor = multiplier * factor + factor;
			}
		}
		return true;
	}

	/*
		Scales each element of a complex vector by 1/N, where
		N is the number of elements in the vector. Scaling should 
		only occur once when performing both FFT and IFFT.

		data - vector to scale

		Returns true on success and false on failure.
	*/
	bool scale(std::vector<std::complex<double>> &data)
	{
		const unsigned int length{ static_cast<unsigned int>(data.size()) };
		// input validation
		if (length == 0)
		{
			std::cerr << "FFT::scale(): invalid vector length\n";
			return false;
		}
		const double scaleFactor{ 1.0 / length };
		for (auto &element : data)
		{
			element *= scaleFactor;
		}
		return true;
	}

	/*
		Performs radix-2 Cooley-Tukey Fast Fourier Transform on a
		complex vector. Length of vector should be a power of 2.

		samples - vector to transform
		dataOut - vector to hold transformed samples
		(length of samples must equal length of dataOut)
		scaleData - scale vector elements by 1/N  where
		N is the number of elements in the vector.

		Returns true on success and false on failure.
	*/
	bool forward(const std::vector<std::complex<double>> &samples, std::vector<std::complex<double>> &dataOut, bool scaleData)
	{
		unsigned int length{ static_cast<unsigned int>(samples.size()) };
		// input validation
		if ((length != dataOut.size()) || (length == 0) || (length & (length - 1)))
		{
			std::cerr << "FFT::forward(): invalid vector length\n";
			return false;
		}
		if (!reverseArrange(samples, dataOut))
		{
			std::cerr << "FFT::forward(): call to reverseArrange() returned false\n";
			return false;
		}
		if (!transform(dataOut, false))
		{
			std::cerr << "FFT::forward(): call to transform() returned false\n";
			return false;
		}
		if (scaleData)
		{
			if (!scale(dataOut))
			{
				std::cerr << "FFT::forward(): call to scale() returned false\n";
				return false;
			}
		}
		return true;
	}

	/*
		Performs radix-2 Cooley-Tukey Fast Fourier Transform on a complex
		vector in place. Length of vector should be a power of 2.

		samples - vector to transform
		scaleData - scale vector elements by 1/N  where
		N is the number of elements in the vector.

		Returns true on success and false on failure.
	*/
	bool forward(std::vector<std::complex<double>> &samples, bool scaleData)
	{
		unsigned int length{ static_cast<unsigned int>(samples.size()) };
		// input validation
		if ((length == 0) || (length & (length - 1)))
		{
			std::cerr << "FFT::forward(): invalid vector length\n";
			return false;
		}
		if (!reverseArrange(samples))
		{
			std::cerr << "FFT::forward(): call to reverseArrange() returned false\n";
			return false;
		}
		if (!transform(samples, false))
		{
			std::cerr << "FFT::forward(): call to transform() returned false\n";
			return false;
		}
		if (scaleData)
		{
			if (!scale(samples))
			{
				std::cerr << "FFT::forward(): call to scale() returned false\n";
				return false;
			}
		}
		return true;
	}

	/*
		Performs radix-2 Cooley-Tukey Inverse Fast Fourier Transform
		on a complex vector. Length of vector should be a power of 2.

		spectrum - vector to transform
		dataOut - vector to hold transformed spectrum
		(length of spectrum must equal length of dataOut)
		scaleData - scale vector elements by 1/N  where
		N is the number of elements in the vector.

		Returns true on success and false on failure.
	*/
	bool inverse(const std::vector<std::complex<double>> &spectrum, std::vector<std::complex<double>> &dataOut, bool scaleData)
	{
		unsigned int length{ static_cast<unsigned int>(spectrum.size()) };
		// input validation
		if ((length != dataOut.size()) || (length == 0) || (length & (length - 1)))
		{
			std::cerr << "FFT::inverse(): invalid vector length\n";
			return false;
		}
		if (!reverseArrange(spectrum, dataOut))
		{
			std::cerr << "FFT::inverse(): call to reverseArrange() returned false\n";
			return false;
		}
		if (!transform(dataOut, true))
		{
			std::cerr << "FFT::inverse(): call to transform() returned false\n";
			return false;
		}
		if (scaleData)
		{
			if (!scale(dataOut))
			{
				std::cerr << "FFT::inverse(): call to scale() returned false\n";
				return false;
			}
		}
		return true;
	}

	/*
		Performs radix-2 Cooley-Tukey Inverse Fast Fourier Transform on a
		complex vector in place. Length of vector should be a power of 2.

		spectrum - vector to transform
		scaleData - scale vector elements by 1/N  where
		N is the number of elements in the vector.

		Returns true on success and false on failure.
	*/
	bool inverse(std::vector<std::complex<double>> &spectrum, bool scaleData)
	{
		unsigned int length{ static_cast<unsigned int>(spectrum.size()) };
		// input validation
		if ((length == 0) || (length & (length - 1)))
		{
			std::cerr << "FFT::inverse(): invalid vector length\n";
			return false;
		}
		if (!reverseArrange(spectrum))
		{
			std::cerr << "FFT::inverse(): call to reverseArrange() returned false\n";
			return false;
		}
		if (!transform(spectrum, true))
		{
			std::cerr << "FFT::inverse(): call to transform() returned false\n";
			return false;
		}
		if (scaleData)
		{
			if (!scale(spectrum))
			{
				std::cerr << "FFT::inverse(): call to scale() returned false\n";
				return false;
			}
		}
		return true;
	}
}
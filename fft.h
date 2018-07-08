#ifndef FFT_H
#define FFT_H

#include <vector>
#include <complex>

namespace FFT
{
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
	bool forward(const std::vector<std::complex<double>> &samples, std::vector<std::complex<double>> &dataOut, bool scaleData = true);

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
	bool inverse(const std::vector<std::complex<double>> &spectrum, std::vector<std::complex<double>> &dataOut, bool scaleData = false);

	// IN PLACE VERSIONS

	/*
		Performs radix-2 Cooley-Tukey Fast Fourier Transform on a complex
		vector in place. Length of vector should be a power of 2.

		samples - vector to transform
		scaleData - scale vector elements by 1/N  where
		N is the number of elements in the vector.

		Returns true on success and false on failure.
	*/
	bool forward(std::vector<std::complex<double>> &samples, bool scaleData = true);

	/*
		Performs radix-2 Cooley-Tukey Inverse Fast Fourier Transform on a
		complex vector in place. Length of vector should be a power of 2.

		spectrum - vector to transform
		scaleData - scale vector elements by 1/N  where
		N is the number of elements in the vector.

		Returns true on success and false on failure.
	*/
	bool inverse(std::vector<std::complex<double>> &spectrum, bool scaleData = false);
}
#endif
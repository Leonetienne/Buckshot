#pragma once
#include <png.h>
#include <string>
#include <exception>

/// <summary>
/// PNG-Image wrapper for libpng. Only supports rgb-8bit because we just don't need anything higher for this project
/// </summary>
class PNGImage
{
public:
	// Will create an empty image of size width, height. Pixel buffer will be created, but zeroed
	PNGImage(const std::size_t width, const std::size_t height);

	// Will create a new image of size width, height and copy the pixel buffer given
	PNGImage(const std::size_t width, const std::size_t height, const unsigned char* pixelBuffer);

	// Copy-Constructor
	PNGImage(const PNGImage& other);

	~PNGImage();

	// Will return the raw pixel buffer used. Format is rgbrgbrgbrgbrgbrgbrgb... All rows concatenated
	unsigned char* GetPixelBuffer() { return pixelBuffer; }
	// Will return the raw pixel buffer used. Format is rgbrgbrgbrgbrgbrgbrgb... All rows concatenated
	const unsigned char* GetPixelBuffer() const { return pixelBuffer; }

	// Will copy the pixels from a pixel buffer. Make sure the buffer is of size width*height*3
	void CopyPixelBuffer(const unsigned char* pixelBuffer);

	// Will return an array of pointers to the r, g and b values of the pixel specified
	inline unsigned char** At(std::size_t x, std::size_t y)
	{
		if ((x >= width) || (y >= height)) throw std::out_of_range("Pixel coords out of range");

		std::size_t pixelIndex = (y * width * 3) + (x * 3);

		unsigned char* rgb[3] = {
			pixelBuffer + pixelIndex + 0,
			pixelBuffer + pixelIndex + 1,
			pixelBuffer + pixelIndex + 2,
		};
		return rgb;
	}
	// Will return an array of pointers to the r, g and b values of the pixel specified
	inline const unsigned char** At(std::size_t x, std::size_t y) const
	{
		if ((x >= width) || (y >= height)) throw std::out_of_range("Pixel coords out of range");

		std::size_t pixelIndex = (y * width * 3) + (x * 3);

		unsigned const char* rgb[3] = {
			pixelBuffer + pixelIndex + 0,
			pixelBuffer + pixelIndex + 1,
			pixelBuffer + pixelIndex + 2,
		};
		return rgb;
	}

	// Saves the image to a file. Returns false if fails. Check PNGImage::GetLastError()
	bool SaveToFile(std::string filename);

	std::string GetLastError() { return lastError; };

private:
	unsigned char* pixelBuffer;
	std::size_t width;
	std::size_t height;

	std::string lastError;
};


#include "PNGImage.h"

PNGImage::PNGImage(const std::size_t width, const std::size_t height)
{
	this->width = width;
	this->height = height;
	
	pixelBuffer = new unsigned char[width * height * 3];
	memset(pixelBuffer, 0, sizeof(*pixelBuffer));
}

PNGImage::PNGImage(const std::size_t width, const std::size_t height, const unsigned char* pixelBuffer)
{
	this->width = width;
	this->height = height;

	this->pixelBuffer = new unsigned char[width * height * 3];
	CopyPixelBuffer(pixelBuffer);
}

PNGImage::~PNGImage()
{
	delete[] pixelBuffer;
	pixelBuffer = nullptr;
}

void PNGImage::CopyPixelBuffer(const unsigned char* pixelBuffer)
{
	memcpy(this->pixelBuffer, pixelBuffer, sizeof(unsigned char) * width * height * 3);
	return;
}

bool PNGImage::SaveToFile(std::string filename)
{
	png_byte p;
	png_infop info;
	png_structp png;
	png_bytep* row_pointers;


	// Initialize libpng structs
	png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png)
	{
		lastError = "Unable to create png write struct!";
		return false;
	}

	info = png_create_info_struct(png);
	if (!info)
	{
		lastError = "Unable to create png info struct!";
		return false;
	}

	if (setjmp(png_jmpbuf(png)))
	{
		lastError = "Error during init_io!";
		return false;
	}

	FILE* file = fopen(filename.c_str(), "wb");
	png_init_io(png, file);


	// Write png header
	if (setjmp(png_jmpbuf(png)))
	{
		lastError = "Error during writing header!";
		return false;
	}
	png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(png, info);

	
	// Create libpng pixel buffer
	row_pointers = (png_bytep*)calloc(height, sizeof(png_bytep));
	for (std::size_t i = 0; i < height; i++)
	{
		row_pointers[i] = (png_byte*)calloc(width * 3, sizeof(png_bytep));
		memcpy(row_pointers[i], pixelBuffer + (width * 3 * i), sizeof(unsigned char) * width * 3);
	}


	// Write png bytes
	if (setjmp(png_jmpbuf(png)))
	{
		lastError = "Error during writing bytes!";
		return false;
	}
	png_write_image(png, row_pointers);


	// End write
	if (setjmp(png_jmpbuf(png)))
	{
		lastError = "Error during end of write!";
		return false;
	}
	png_write_end(png, NULL);


	// Cleanup
	for (std::size_t i = 0; i < height; i++)
	{
		free(row_pointers[i]);
	}
	free(row_pointers);

	return false;
}

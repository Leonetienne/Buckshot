#include <iostream>
#include "PNGImage.h"

void CreateDummyImage(PNGImage& img, std::size_t width, std::size_t height)
{
    for (std::size_t y = 0; y < height; y++)
    {
        for (std::size_t x = 0; x < width; x++)
        {
            unsigned char** pixel = img.At(x, y);
            unsigned char& r = *pixel[0];
            unsigned char& g = *pixel[1];
            unsigned char& b = *pixel[2];

            double bwval = sin(x / 10.0) * cos(y / 10.0) * 255;
            r = bwval;
            g = bwval;
            b = bwval;
        }
    }
    return;
}

int main()
{
    // Create png structs
    
    std::size_t width = 1920;
    std::size_t height = 1080;
    unsigned char* buffer = new unsigned char[width * height * 3];

    PNGImage img(width, height, buffer);
    CreateDummyImage(img, width, height);
    img.SaveToFile("test.png");

    std::cout << "Hello World!\n";
    return 0;
}

#include <iostream>
#include <png.h>

void CreateDummyImage(png_bytep* row_pointers, std::size_t width, std::size_t height)
{
    for (std::size_t y = 0; y < height; y++)
    {
        png_byte* row = row_pointers[y];

        for (std::size_t x = 0; x < width; x++)
        {
            unsigned char& r = row[(x * 3) + 0];
            unsigned char& g = row[(x * 3) + 1];
            unsigned char& b = row[(x * 3) + 2];
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
    png_byte p;
    png_infop info;
    png_structp png;
    png_bytep* row_pointers;
    std::size_t width = 1920;
    std::size_t height = 1080;

    png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
    {
        std::cerr << "Unable to create png write struct!" << std::endl;
        return -1;
    }

    info = png_create_info_struct(png);
    if (!info)
    {
        std::cerr << "Unable to create png info struct!" << std::endl;
        return -1;
    }

    if (setjmp(png_jmpbuf(png)))
    {
        std::cerr << "Error during init_io!" << std::endl;
        return -1;
    }
    FILE* file = fopen("test.png", "wb");
    png_init_io(png, file);


    // Write png header
    if (setjmp(png_jmpbuf(png)))
    {
        std::cerr << "Error during writing header!" << std::endl;
        return -1;
    }
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_write_info(png, info);


    // Create image
    row_pointers = (png_bytep*)calloc(height, sizeof(png_bytep));
    for (std::size_t i = 0; i < height; i++)
        row_pointers[i] = (png_byte*)calloc(width * 3, sizeof(png_bytep));
    CreateDummyImage(row_pointers, width, height);

    // Write png bytes
    if (setjmp(png_jmpbuf(png)))
    {
        std::cerr << "Error during writing bytes!" << std::endl;
        return -1;
    }
    png_write_image(png, row_pointers);

    // End write
    if (setjmp(png_jmpbuf(png)))
    {
        std::cerr << "Error during end of write!" << std::endl;
        return -1;
    }
    png_write_end(png, NULL);

    fclose(file);

    // Cleanup
    for (std::size_t i = 0; i < height; i++)
    {
        free(row_pointers[i]);
    }
    free(row_pointers);

    std::cout << "Hello World!\n";
    return 0;
}

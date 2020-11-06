#include <iostream>
#include <png.h>

int main()
{
    png_byte p;

    png_infop info;
    png_structp png;

    png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    std::cout << "Hello World!\n";
}

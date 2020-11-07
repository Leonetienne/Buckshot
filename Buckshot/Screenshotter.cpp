#include <ctime>
#include <sstream>
#include <iostream>
#include <Windows.h>
#include "Screenshotter.h"
#include "DebugLog.h"

PNGImage Screenshotter::TakeScreenshotOfForegroundWindow()
{
    // Gather window information
    DebugLog("Gathering window information...");
    HWND activeWin = GetForegroundWindow();

    RECT winRect;
    POINT winSize;
    GetWindowRect(activeWin, &winRect);
    winSize.x = winRect.right - winRect.left;
    winSize.y = winRect.bottom - winRect.top;

    // Create bitmap
    DebugLog("Creating bitmap...");
    HDC winDC = GetDC(NULL);
    HDC captureDC = CreateCompatibleDC(winDC);
    HBITMAP captureBitmap = CreateCompatibleBitmap(winDC, winSize.x, winSize.y);
    SelectObject(captureDC, captureBitmap);

    BitBlt(captureDC, 0, 0, winSize.x, winSize.y, winDC, winRect.left, winRect.top, SRCCOPY | CAPTUREBLT);

    BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = winSize.x;
    bmi.bmiHeader.biHeight = winSize.y;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* pixels = new RGBQUAD[winSize.x * winSize.y];
    GetDIBits(captureDC, captureBitmap, 0, winSize.y, pixels, &bmi, DIB_RGB_COLORS);

    DeleteDC(captureDC);
    ReleaseDC(NULL, winDC);


    // Create png image
    DebugLog("Creating png...");
    PNGImage png(winSize.x, winSize.y);
    for (std::size_t x = 0; x < (std::size_t)winSize.x; x++)
    {
        for (std::size_t y = 0; y < (std::size_t)winSize.y; y++)
        {
            unsigned char** pixel = png.At(x, winSize.y - y - 1); // the rgbquad array is for some reason y=0 -> bottom
            *pixel[0] = pixels[(y * winSize.x) + x].rgbRed;
            *pixel[1] = pixels[(y * winSize.x) + x].rgbGreen;
            *pixel[2] = pixels[(y * winSize.x) + x].rgbBlue;
        }
    }

    return png;
}

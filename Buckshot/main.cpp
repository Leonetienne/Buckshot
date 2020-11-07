#include <Windows.h>
#include <iostream>
#include <sstream>
#include <ctime>
#include "PNGImage.h"

std::string GetTimeString()
{
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H-%M-%S", timeinfo);
    return std::string(buffer);
}

std::string SanitizeFilename(const std::string& fname)
{
    std::stringstream ss;

    for (std::size_t i = 0; i < fname.length(); i++)
    {
        const char c = fname[i];
        
        bool keepChar = true;
        if ((c == '<') || (c == '>') || (c == '"') || (c == '/') || (c == '\\') ||
            (c == '|') || (c == '?') || (c == '*') || (c == ':'))
        {
            keepChar = false;
        }

        if (keepChar)
            ss << c;
    }

    return ss.str();
}

void TakeScreenshot()
{
    // Gather window information
    HWND activeWin = GetForegroundWindow();

    RECT winRect;
    POINT winSize;
    GetWindowRect(activeWin, &winRect);
    winSize.x = winRect.right - winRect.left;
    winSize.y = winRect.bottom - winRect.top;

    LPSTR winTitle = new char[255];
    GetWindowTextA(activeWin, winTitle, 255);
    //std::cout << "Found window: " << winTitle << std::endl;

    // Create bitmap
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

    //std::cout << "Creating and saving png..." << std::endl;

    // Create png image
    PNGImage png(winSize.x, winSize.y);
    for (std::size_t x = 0; x < winSize.x; x++)
    {
        for (std::size_t y = 0; y < winSize.y; y++)
        {
            unsigned char** pixel = png.At(x, winSize.y - y - 1);
            *pixel[0] = pixels[(y * winSize.x) + x].rgbRed;
            *pixel[1] = pixels[(y * winSize.x) + x].rgbGreen;
            *pixel[2] = pixels[(y * winSize.x) + x].rgbBlue;
        }
    }

    // Save to file
    CreateDirectoryA("screenshots", NULL);
    std::stringstream filename;
    filename << winTitle << " - " << GetTimeString() << ".png";
    //std::cout << filename.str() << std::endl;
    png.SaveToFile(std::string("screenshots\\") + SanitizeFilename(filename.str()));

    //std::cout << "Saved png!" << std::endl;


    delete winTitle;
    return;
}

int main()
{
    // Windows specific bullshit
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
    //ShowWindow(GetConsoleWindow(), SW_HIDE);

    // Actual program
    bool isRunning = true;
    bool buttonLocked = false;

    while (isRunning)
    {
        if ((GetAsyncKeyState(VK_LSHIFT)) &&
            (GetAsyncKeyState(VK_LCONTROL)) &&
            (GetAsyncKeyState(VK_DELETE)))
        {
            if (!buttonLocked)
            {
                buttonLocked = true;
                TakeScreenshot();
            }
        }
        else
        {
            buttonLocked = false;

            // Let the processor chill
            Sleep(100);
        }
    }

    return 0;
}

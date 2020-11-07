#pragma once
#include <string>
#include <ctime>
#include <Windows.h>

inline std::string GetForegroundWindowTitle()
{
    HWND activeWin = GetForegroundWindow();
    std::string winTitle;
    LPSTR tmp = new char[255];
    GetWindowTextA(activeWin, tmp, 255);
    winTitle = std::string(tmp);
    delete tmp;
    return winTitle;
}

inline std::string GetTimeString()
{
    time_t rawtime;
    struct tm* timeinfo;
    char buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H-%M-%S", timeinfo);
    return std::string(buffer);
}

std::string SanitizeFilename(std::string filename)
{
    std::stringstream ss;

    for (std::size_t i = 0; i < filename.length(); i++)
    {
        const char c = filename[i];

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

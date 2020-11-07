#include <Windows.h>
#include <iostream>
#include <sstream>
#include "Screenshotter.h"
#include "SetupWindowsEnviroment.h"
#include "Util.h"
#include "DebugLog.h"

void TakeScreenshot()
{
    std::string currWindowTitle = GetForegroundWindowTitle();

    DebugLog(std::string("Found foreground window ") + currWindowTitle);
    
    DebugLog(std::string("Taking screenshot of ") + currWindowTitle);
    PNGImage newScreenshot = Screenshotter::TakeScreenshotOfForegroundWindow();

    CreateDirectoryA("screenshots", NULL);
    std::stringstream filename;
    filename << currWindowTitle << " - " << GetTimeString() << ".png";
    
    DebugLog(std::string("Saving screenshot to ") + filename.str());

    if (!newScreenshot.SaveToFile(std::string("screenshots\\") + SanitizeFilename(filename.str())))
    {
        std::cerr << "Failed to save screenshot: " << newScreenshot.GetLastError() << std::endl;
    }

    return;
}

int main()
{
    // Windows specific bullshit
    SetupWindowsEnviroment();

    DebugLog("This is a debug build!");
    DebugLog("Make sure to run a release build for the best performance!");

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

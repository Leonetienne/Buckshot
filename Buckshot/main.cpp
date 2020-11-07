#include <Windows.h>
#include <iostream>
#include <sstream>
#include "Screenshotter.h"
#include "SetupWindowsEnviroment.h"
#include "Util.h"
#include "Config.h"
#include "DebugLog.h"

void TakeScreenshot()
{
    std::string currWindowTitle = GetForegroundWindowTitle();

    DebugLog(std::string("Found foreground window ") + currWindowTitle);

    bool shouldTakeScreenshot = false;
    if (Config::GetSettings().useWhiteListInstead)
    {
        bool isWhitelisted = false;
        for (std::size_t i = 0; i < Config::GetSettings().appWhitelist.size(); i++)
        {
            if (currWindowTitle == Config::GetSettings().appWhitelist[i])
            {
                isWhitelisted = true;
                break;
            }
        }
        if (!isWhitelisted) DebugLog("Not taking screenshot... Not whitelisted!");
        shouldTakeScreenshot = isWhitelisted;
    }
    else // use blacklist
    {
        bool isBlacklisted = false;
        for (std::size_t i = 0; i < Config::GetSettings().appBlacklist.size(); i++)
        {
            if (currWindowTitle == Config::GetSettings().appBlacklist[i])
            {
                isBlacklisted = true;
                DebugLog("Not taking screenshot... Blacklisted!");
                break;
            }
        }
        shouldTakeScreenshot = !isBlacklisted;
    }
    
    if (shouldTakeScreenshot)
    {
        DebugLog(std::string("Taking screenshot of ") + currWindowTitle);
        PNGImage newScreenshot = Screenshotter::TakeScreenshotOfForegroundWindow();

        std::string savePath = Config::GetSettings().defaultSavePath;
        for (std::size_t i = 0; i < Config::GetSettings().savePathsPerApp.size(); i++)
        {
            if (currWindowTitle == Config::GetSettings().savePathsPerApp[i].first)
            {
                savePath = Config::GetSettings().savePathsPerApp[i].second;
                DebugLog("Found override save path for this app!");
                break;
            }
        }

        std::string namingScheme = Config::GetSettings().defaultNamingScheme;
        for (std::size_t i = 0; i < Config::GetSettings().namingSchemePerApp.size(); i++)
        {
            if (currWindowTitle == Config::GetSettings().namingSchemePerApp[i].first)
            {
                namingScheme = Config::GetSettings().namingSchemePerApp[i].second;
                DebugLog("Found override naming scheme for this app!");
                break;
            }
        }

        // If we use the default saving directory, create it. It's just a single folder. No complicated path
        if (savePath == "screenshots")
        {
            CreateDirectoryA("screenshots", NULL);
        }
        std::string filename = NamingSchemeToFilename(namingScheme, currWindowTitle);
        std::string absoluteFilename = RemoveTrailingSlashFromPath(savePath) + "\\" + filename;

        DebugLog(std::string("Saving screenshot to ") + absoluteFilename);
        
        if (!newScreenshot.SaveToFile(absoluteFilename))
        {
            std::cerr << "Failed to save screenshot: " << newScreenshot.GetLastError() << std::endl;
            MessageBoxA(0, (std::string("Failed to save screenshot: ") + newScreenshot.GetLastError()).c_str(), "Buckshot: Can't save screenshot!", MB_ICONERROR | MB_OK);
        }
        else
        {
            DebugLog("Saved succesfully!\n");
        }
    }

    return;
}

int main()
{
    ForceSingleInstance(); // Don't allow to open this program twice

    Config config; // This instance sets up the static function Config::GetSettings()

    // Windows specific bullshit
    SetupWindowsEnviroment();

#ifdef DEBUG_BUILD
    std::cout << "This is a debug build!" << std::endl
              << "Make sure to run a release build for the best performance!" << std::endl;
#endif

    // Actual program
    bool isRunning = true;
    bool buttonLocked = false;

    while (isRunning)
    {
        if (IsKeybindPressed())
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

#pragma once
#include <string>
#include <ctime>
#include <Windows.h>
#include <tlhelp32.h>
#include "Config.h"
#include "Dependencies/JasonPP.hpp"

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

inline std::string GetUnixtimeAsString()
{
    std::stringstream ss;
    ss << time(NULL);
    return ss.str();
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

bool IsKeybindPressed()
{
    bool isAnyNotPressed = false;

    for (std::size_t i = 0; i < Config::GetSettings().keybind.size(); i++)
        if (!GetAsyncKeyState(Config::GetSettings().keybind[i])) isAnyNotPressed = true;

    return !isAnyNotPressed;
}

std::string NamingSchemeToFilename(const std::string& scheme, const std::string& windowTitle)
{
    std::string baseName = scheme;

    baseName = JasonPP::Internal::StringHelpers::Replace(baseName, "$(winName)", windowTitle);
    baseName = JasonPP::Internal::StringHelpers::Replace(baseName, "$(time)", GetTimeString());
    baseName = JasonPP::Internal::StringHelpers::Replace(baseName, "$(unixtime)", GetUnixtimeAsString());

    return SanitizeFilename(baseName);
}

inline bool DoesDirectoryExist(const std::string& dirName)
{
    DWORD ftyp = GetFileAttributesA(dirName.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
        return false;  //something is wrong with your path!

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
        return true;   // this is a directory!

    return false;    // this is not a directory!
}

inline std::string RemoveTrailingSlashFromPath(std::string path)
{
    if (path[path.length() - 1] == '\\')
    {
        return path.substr(0, path.length() - 1);
    }
    return path;
}

HANDLE GetOtherProcessByName(std::wstring name)
{
    DWORD pid = 0;

    // Create toolhelp snapshot.
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 process;
    ZeroMemory(&process, sizeof(process));
    process.dwSize = sizeof(process);

    // Walkthrough all processes.
    if (Process32First(snapshot, &process))
    {
        do
        {
            // Compare process.szExeFile based on format of name, i.e., trim file path
            // trim .exe if necessary, etc.
            if (std::wstring(process.szExeFile) == name)
            {
                if (process.th32ProcessID != GetCurrentProcessId())
                {
                    pid = process.th32ProcessID;
                    break;
                }
            }
        } while (Process32Next(snapshot, &process));
    }

    CloseHandle(snapshot);

    if (pid != 0)
    {
        return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    }

    // Not found


    return NULL;
}

void ForceSingleInstance()
{
    HANDLE procId = GetOtherProcessByName(L"Buckshot.exe");
    if (procId)
    {
        int i = MessageBoxA(NULL, "Buckshot is already open!\nDo you want to close the other instance instead of quitting?", "Buckshot is already open!", MB_ICONQUESTION | MB_YESNO);
        
        if (i == IDYES)
        {
            TerminateProcess(procId, 0);
        }
        else
        {
            exit(-1);
        }
    }

    return;
}

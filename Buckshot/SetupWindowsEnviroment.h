#pragma once
#include <Windows.h>
#include "Config.h"

inline void SetupWindowsEnviroment()
{
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
	SetConsoleTitleA("Buckshot");

#ifdef RELEASE_BUILD
	// Hide the console if the config file says so
	if (!Config::GetSettings().showDebugConsole)
		ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	return;
}

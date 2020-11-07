#pragma once
#include <Windows.h>

inline void SetupWindowsEnviroment()
{
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);

#ifdef RELEASE_BUILD
	// Hide the console
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	return;
}

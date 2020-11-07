#pragma once
#include <iostream>
#include <string>
#include "Config.h"

template <typename T>
inline void DebugLog(const T str)
{
	bool isDebug = false;

#ifdef DEBUG_BUILD
	isDebug = true;
#else
	isDebug = Config::GetSettings().showDebugConsole;
#endif
	
	if (isDebug)
	{
		std::cout << str << std::endl;
	}
	return;
}

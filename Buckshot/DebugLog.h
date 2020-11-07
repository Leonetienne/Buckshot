#pragma once
#include <iostream>
#include <string>

template <typename T>
inline void DebugLog(const T str)
{
#ifdef DEBUG_BUILD
		std::cout << str << std::endl;
#endif
}

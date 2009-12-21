#pragma once
#include <windows.h>

class DllInstance
{
public:
	static HINSTANCE& GetInstance();
};

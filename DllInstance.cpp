#include "DllInstance.h"

HINSTANCE& DllInstance::GetInstance()
{
	static HINSTANCE instance;
	return instance;
}

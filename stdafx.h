// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// STL Containers.
#include <vector>
#include <set>

// Streams.
#include <fstream>

// Winsock.
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

// Platform-specific.
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

// Standard C++.
#include <cassert>
#include <string>

// DirectX.
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// Application-specific.
#include "defs.h"
#include "IHash.h"
#include "Detours/CDetour.h"
#include "NOP.h"
#include "Core.h"

// The ONCE delegate macro.
#define ONCE_HELPER(instructionsToRunOnce, onceVar)	\
	{												\
		static bool onceVar = true;					\
													\
		if(onceVar)									\
		{											\
			onceVar = false;						\
			instructionsToRunOnce					\
		}											\
	}

// Use this macro to run a block of code only the first time it is called.
#define ONCE(instructionsToRunOnce) \
	ONCE_HELPER(instructionsToRunOnce, __onceVar__##__COUNTER__)

// Asserts that optimize in release mode. DISABLED DUE TO INTEL CPP BEING STUPID.
#define ASSERTUME	__noop

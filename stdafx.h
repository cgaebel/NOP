// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

// CRT.
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <malloc.h>

// STL Containers.
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <stack>
#include <map>
#include <set>
#include <bitset>

// STL Extensions.
#include <algorithm>
#include <iterator>
#include <complex>
#include <valarray>
#include <numeric>

// STL Utilities.
#include <utility>
#include <functional>
#include <memory>

// Streams.
#include <iostream>
#include <fstream>
#include <sstream>

// Platform-specific.
#include <intrin.h>

// Winsock.
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

// Platform-specific.
#include <windows.h>
#include <psapi.h>
#include <omp.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <wincodec.h>
#include <objidl.h>
#include <Aclapi.h>
#include <tlhelp32.h>
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "Advapi32.lib")

// Standard C++.
#include <cassert>
#include <cstring>
#include <string>
#include <exception>
#include <typeinfo>
#include <limits>
#include <stdexcept>
#include <locale>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iomanip>

// System stats.
#include <sys/stat.h>

// DirectX.
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

// Application-specific.
#include "./Hash/HMD6.h"
#include "HideThreadFromDebugger.h"

#ifdef _DEBUG
	#pragma comment(lib, "./Hash/Hash.debug.lib")
#else
	#pragma comment(lib, "./Hash/Hash.release.lib")
#endif

// Helps with assembly call tables.
#define DEF_CALL(address)			static void* __call__##address = (void*)address
#define CALL_FROM_TABLE(address)	CALL [__call__##address]

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

// Asserts that optimize in release mode.
#define ASSERTUME(expression)	\
	assert(expression);			\
	__assume(expression)

#pragma warning(disable:6320)
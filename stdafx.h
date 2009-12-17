// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// The following macros define the minimum required platform.  The minimum required platform
// is the earliest version of Windows, Internet Explorer etc. that has the necessary features to run 
// your application.  The macros work by enabling all features available on platform versions up to and 
// including the version specified.

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER					// Specifies that the minimum required platform is Windows XP.
#define WINVER 0x0501           // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT            // Specifies that the minimum required platform is Windows XP.
#define _WIN32_WINNT 0x0501     // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS          // Specifies that the minimum required platform is Windows 98.
#define _WIN32_WINDOWS 0x0410	// Change this to the appropriate value to target Windows Me or later.
#endif

// STL.
#include <vector>
#include <set>
#include <algorithm>
#include <regex>
#include <memory>
#include <sstream>

// Streams.
#include <fstream>

// Winsock.
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

// Platform-specific.
#include <WinNT.h>
#include <windows.h>
#include <dbghelp.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

// Standard C.
#include <stddef.h>
#include <strsafe.h>

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
#include "Utilities.h"
#include "DllInstance.h"

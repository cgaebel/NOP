#pragma once

typedef LONG NTSTATUS;

#define CODE_SEGMENT_BEGIN	((BYTE*)0x00401000)
//#define CODE_SEGMENT_BEGIN	((BYTE*)0x004437FF)	/* used to be 0x004F37FF */
#define CODE_SEGMENT_END	((BYTE*)0x005E5FFF)

#define MODULE_SPACE_BEGIN	((BYTE*)0x70000000)
#define MODULE_SPACE_END	((BYTE*)0x7FFFFFFF)

#define FAILURE_FILELOG \
			"NOP.log"

#define HACKLOG_FILENAME \
			"NOP.log"

#define MEMORY_HASH_FAILED_FILENAME	\
			"Invalid Memory Hash.txt"

#define FILE_HASH_FAILED_FILENAME \
			"Invalid File Hash.txt"

#define API_FUNCTIONS \
			OpenThread, CloseHandle, CreateThread, LoadLibraryA,			\
			GetProcAddress, TerminateProcess, VirtualProtect, OpenProcess,	\
			GetModuleHandleA, ExitProcess, VirtualAlloc, VirtualFree

#define PROLOGUE_CODE \
			0x8B

#define CORRECT_INVALID_THREAD_COUNT	0
#define CORRECT_HOOKED_API_COUNT		0
#define CORRECT_INVALID_MODULE_COUNT	0

#define INVALID_THREAD_COUNT_FILENAME \
			"Invalid External Thread Count.txt"

#define INVALID_API_HOOK_COUNT_FILENAME \
			"Invalid Hook Count.txt"

#define INVALID_MODULE_COUNT_FILENAME \
			"Invalid Module Count.txt"

#define TRAINER_WINDOW_TITLE_BLACKLIST	\
			"ZxPwd",					\
			"injector",					\
			"*REMADE*"

#define FILE_CHECKSUM_PROTECTED_FILES	\
			"svchost.exe",				\
			"NOP.dll",					\
			"dbghelp.dll",				\
			"fmod.dll",					\
			"gdiplus.dll",				\
			"HanAuthForClient.dll",		\
			"HanPollForClient.dll",		\
			"HanReportForClient.dll",	\
			"d3d9.dll",					\
			"d3dx9_41.dll",				\
			"System.xFC",				\
			"GunzLauncher.exe"

#define DLL_NAME \
			"NOP.dll"

// The maximum amount of time an exception should take to process.
#define EXCEPTION_DELTA_IN_MILLISECONDS \
			100

#define OVERLAY_TEXT \
			"Powered by NOP Antihack\n"	\
			"Created by Clark \"wowus\" Gaebel"

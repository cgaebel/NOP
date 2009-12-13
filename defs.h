#pragma once

typedef LONG NTSTATUS;

#define CODE_SEGMENT_BEGIN	((BYTE*)0x00401000)
#define CODE_SEGMENT_END	((BYTE*)0x005E5FAA)

#define FAILURE_FILELOG		\
			"NOP.log"

#define HACKLOG_FILENAME	\
			"NOP.log"

#define INFO_FILENAME		\
			"NOP.log"

#define TRAINER_WINDOW_TITLE_BLACKLIST	\
			"ZxPwd",					\
			"injector",					\
			"*REMADE*",					\
			"cheatengine"

#define DLL_NAME \
			"NOP.dll"

#define FILE_CHECKSUM_PROTECTED_FILES	\
			"svchost.exe",				\
			DLL_NAME,					\
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

// In order to update the date, you MUST do a rebuild all.
#define OVERLAY_TEXT \
			"Protected by NOP " __DATE__ "\n"	\
			"Created by Clark Gaebel"

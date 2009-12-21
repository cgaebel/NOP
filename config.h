#pragma once

///////////////////// MEMORY SCANNING /////////////////////

#define CODE_SEGMENT_BEGIN	((BYTE*)0x00401000)
#define CODE_SEGMENT_END	((BYTE*)0x005E5FAA)

#define HASH_LOCATION		"http://cloud.github.com/downloads/wowus/SuperSecretNinjaProject/hashinfo.v2"

///////////////////// LOGGING /////////////////////

#define FAILURE_FILELOG					\
			"NOP.log"

#define HACKLOG_FILENAME				\
			"NOP.log"

#define INFO_FILENAME					\
			"NOP.log"

///////////////////// MRS /////////////////////

#define MRS_KEY							\
			"\x4F"

///////////////////// PROTECTION /////////////////////

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

///////////////////// OVERLAY /////////////////////

// MUST BE LESS THAN 16 CHARACTERS LONG.
#define OVERLAY_TEXT							\
			"0x90 "

#define GUNZ_PROTOCOL_KEY						\
			"\x90"

// Use Viewport.Width - (your x value here) to denote distance from the right.
#define OVERLAY_POSITION_X						\
			10

// Use Viewport.Height - (your y value here) to denote distance from the bottom.
#define OVERLAY_POSITION_Y						\
			Viewport.Height - 40

///////////////////// CORE DELAY /////////////////////

#define INNER_CORE_LOOP_DELAY	\
			20

#define OUTER_CORE_LOOP_DELAY	\
			2000

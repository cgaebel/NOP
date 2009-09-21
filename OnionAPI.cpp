#include "OnionAPI.h"
#include "FastSystemCall.h"

DWORD GetThreadStartAddress(HANDLE ThreadHandle)
{
	DWORD StartAddress = 0;

	_NtQueryInformationThread(ThreadHandle, 9, &StartAddress, sizeof(DWORD), 0);

	if(!StartAddress)
		_NtQueryInformationThread(ThreadHandle, 8, &StartAddress, sizeof(DWORD), 0);

	return StartAddress;
}

DWORD EnumerateThreads(DWORD ProcessId)
{
	void** SysProcess = NULL;
	HANDLE ThreadHandle = 0;

	CONTEXT ctx;
	ctx.ContextFlags = CONTEXT_FULL;

	VirtualAlloc(&SysProcess, 102400, 0x1000, 0x04);
	_NtQuerySystemInformation(SystemProcessInformation, SysProcess, 102400, 0);

	SYSTEM_PROCESS_INFORMATION *CurrProcess = NULL;

	for(
		CurrProcess = (SYSTEM_PROCESS_INFORMATION*)&SysProcess[0];
		CurrProcess->NextEntryOffset && (DWORD)CurrProcess->ProcessId != ProcessId;
		CurrProcess = (SYSTEM_PROCESS_INFORMATION*)((DWORD)CurrProcess + (DWORD)CurrProcess->NextEntryOffset)
	);

	DWORD BadThreadCount = 0;

	for(unsigned int i = 0; i < CurrProcess->ThreadCount; ++i)
	{
		ThreadHandle = OpenThread(0x48, FALSE, (DWORD)CurrProcess->Threads[i].ClientId.UniqueThread);

		if(ThreadHandle == NULL)
			continue;

		BYTE* ThreadStartAddress = (BYTE*)GetThreadStartAddress(ThreadHandle);
		GetThreadContext(ThreadHandle, &ctx);

		#ifdef _M_IX86
			BYTE* ThreadIP = (BYTE*)ctx.Eip;
		#elif _M_IX64
			BYTE* ThreadIP = (BYTE*)ctx.Rip;
		#endif

		if(
			(ThreadStartAddress > CODE_SEGMENT_BEGIN || ThreadStartAddress < CODE_SEGMENT_END) ||
			(
				(ThreadIP > CODE_SEGMENT_END   || ThreadIP < CODE_SEGMENT_BEGIN) &&
				(ThreadIP < MODULE_SPACE_BEGIN || ThreadIP > MODULE_SPACE_END)
			)
		)
			++BadThreadCount;

		CloseHandle(ThreadHandle);
	}
	return BadThreadCount;
}

DWORD EnumerateFunctions()
{
	const void* ApiFunctions[] = {
		API_FUNCTIONS
	};

	const UCHAR PrologueCode[] = {
		PROLOGUE_CODE
	};

	DWORD BadFunctionCount = 0;

	for(unsigned int i = 0; i < _countof(ApiFunctions); ++i)
		if(memcmp(ApiFunctions[i], PrologueCode, _countof(PrologueCode)))
			++BadFunctionCount;

	return BadFunctionCount;
}

DWORD EnumerateModules()
{
	#if _M_IX86
		PPEB_LDR_DATA pebModuleLdr = (PPEB_LDR_DATA)*((DWORD_PTR*)__readfsdword(0x30) + 12 / sizeof(DWORD_PTR));
	#elif _M_X64
		PPEB_LDR_DATA pebModuleLdr = (PPEB_LDR_DATA)*((DWORD_PTR*)__readgsqword(0x60) + 24 / sizeof(DWORD_PTR));
	#endif

	PLIST_ENTRY pebModuleHeader = (PLIST_ENTRY)&pebModuleLdr->InLoadOrderModuleList;

	PLDR_MODULE lclModule = (PLDR_MODULE)pebModuleHeader->Flink;
	PLIST_ENTRY ModuleLoop = pebModuleHeader->Flink;

	DWORD BadModuleCount = 0;

	while(pebModuleHeader != ModuleLoop->Flink)
	{
		if(
			(
				((BYTE*)(lclModule->EntryPoint) < CODE_SEGMENT_BEGIN) ||
				((BYTE*)(lclModule->EntryPoint) > CODE_SEGMENT_END)
			) &&
			(
				((BYTE*)(lclModule->EntryPoint) < MODULE_SPACE_BEGIN) ||
				((BYTE*)(lclModule->EntryPoint) > MODULE_SPACE_END)
			)
		)
		{
			std::basic_stringstream<wchar_t> ss;
			ss << (DWORD)lclModule->EntryPoint;
			MessageBoxW(NULL, lclModule->BaseDllName.Buffer, ss.str().c_str(), MB_OK);

			++BadModuleCount;
		}

		lclModule = (PLDR_MODULE)ModuleLoop->Flink;
		ModuleLoop = ModuleLoop->Flink;
	}

	return BadModuleCount;
}
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

#include "CProtectionManager.h"

#include <intrin.h>

#include "defs.h"

#pragma warning(disable:6309)
#pragma warning(disable:6387)

class SiberianTiger
{
private:
	DWORD DebugBit;
	DWORD DebugBreakBit;
protected:
	inline void CallDebugBreak()
	{
		DebugBreakBit = 1;
		DebugBreak();
		DebugBreakBit = 0;
	}

	inline void CallSpfDebugBreak()
	{
		DebugBreakBit = 1;
		#ifdef __INTRIN_H_
			__debugbreak();
		#else
			__asm	INT 3;
		#endif
		DebugBreakBit = 0;
	}

	inline void RmHeaderInfo()
	{
		DWORD lpflOldProtect;
		VirtualProtect((void*)0x004000EA, 1024, 0x40, &lpflOldProtect);
		ZeroMemory((void*)0x004000EA, 1024);
		VirtualProtect((void*)0x004000EA, 1024, 0x02040001, &lpflOldProtect);
	}

	inline void SetStackTrap()
	{
		#ifdef _M_IX86
			__asm
			{
				PUSH SS
				POP SS
				PUSHF
				POP ECX
			}
		#endif
	}

	inline DWORD CheckCloseHandle()
	{
		try {
			CloseHandle(HANDLE(-1));
		} catch(...) {
			return 0;
		}

		return 1;
	}

	inline DWORD CheckDebugPort()
	{
		DWORD debugPResult;

		typedef NTSTATUS (*NQIP)(HANDLE, LONG, PVOID, ULONG, PULONG);
		NQIP NtQueryInformationProcess = (NQIP)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryInformationProcess");

		NtQueryInformationProcess(GetCurrentProcess(), 0x7, &debugPResult, 4, 0);
		return (debugPResult == 0);
	}

	inline DWORD CheckDebugProperty()
	{
		DWORD debugPResult;

		typedef NTSTATUS (WINAPI *NQIP)(HANDLE, LONG, PVOID, ULONG, PULONG);
		NQIP NtQueryInformationProcess = (NQIP)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryInformationProcess");

		NtQueryInformationProcess(GetCurrentProcess(), 0x1E, &debugPResult, 4, 0);

		return (debugPResult == 0);
	}

	inline DWORD CheckIsDbgPresent()
	{
		DWORD_PTR *BeingDebuggedBit;
		DWORD_PTR BeingDebuggedBitReal;
		DWORD returnVal = 1;

		if(IsDebuggerPresent() != 0)
			returnVal = 0;

		#ifdef __INTRIN_H_
			#ifdef _M_IX86
				BeingDebuggedBit = (DWORD_PTR*)__readfsdword(0x30);
			#elif _M_X64
				BeingDebuggedBit = (DWORD_PTR*)__readgsqword(0x60);
			#endif
		#else
			#ifdef _M_IX86
				__asm {
					MOV EAX, FS:[0x30]
					MOV BeingDebuggedBit, EAX
				};
			#endif
		#endif
		BeingDebuggedBitReal = *BeingDebuggedBit;
		*BeingDebuggedBit = 0x00010000;

		if(IsDebuggerPresent() == 0)
			returnVal = 0;

		*BeingDebuggedBit = BeingDebuggedBitReal;

		return returnVal;
	}

	inline DWORD CheckPrefix()
	{
		#ifdef _M_IX86
			try {
				__asm
				{
					__emit 0xF3
					__emit 0x64
					__emit 0xF1
				}
			} catch (...) {
				return 1;
			}
		#endif

		return 0;
	}

	inline DWORD CheckRemoteDebugger()
	{
		DWORD returnVal = 0;
		CheckRemoteDebuggerPresent(GetCurrentProcess(), (PBOOL)&returnVal);
		return (returnVal == 0);
	}

	inline DWORD CheckSeDebugPrivilege()
	{
		typedef DWORD (NTAPI *CGPID)();
		CGPID CsrGetProcessId = (CGPID)GetProcAddress(GetModuleHandle("ntdll.dll"), "CsrGetProcessId");
		if(CsrGetProcessId == 0)
			return 1;

		return (OpenProcess(PROCESS_ALL_ACCESS, FALSE, CsrGetProcessId()) <= 0);
	}

	inline DWORD CheckTrapFlag()
	{
		DWORD TrapFlagStatus;

		#ifdef __INTRIN_H_
			TrapFlagStatus = __readeflags();
		#else
			__asm
			{
				PUSHF
				POP EAX
				MOV TrapFlagStatus, EAX
				POPFD
			}
		#endif

		return ((TrapFlagStatus & 0x0FEFF) == TrapFlagStatus);
	}

	inline DWORD OutputDebugCheck()
	{
		OSVERSIONINFO osvi;

		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if(GetVersionEx(&osvi) == 0)
			return 0;

		if(osvi.dwMajorVersion < 6)
		{
			SetLastError(0x0F0F0F0F);
			OutputDebugString("%s%s");
			if(GetLastError() == 0x0F0F0F0F)
				return 0;
		}
		else
		{
			try {
				OutputDebugString("%s%s");
			} catch(...) {
				return 0;
			}
		}

		return 1;
	}

	inline DWORD GetBeingDebugged()
	{
		DWORD_PTR* BeingDebuggedBit;
		#ifdef __INTRIN_H_
			#ifdef _M_IX86
				BeingDebuggedBit = (DWORD_PTR *) __readfsdword(0x30);
			#elif _M_X64
				BeingDebuggedBit = (DWORD_PTR *) __readgsqword(0x60);
			#endif
			return ((DWORD)*BeingDebuggedBit ^ 0x00010000);
		#else
			#ifdef _M_IX86
				__asm
				{
					MOV EAX, FS:[0x30]
					MOV BeingDebuggedBit, EAX
				}
			#endif
			return ((DWORD)*BeingDebuggedBit ^ 0x00010000);
		#endif
	}

	inline DWORD GetDebugRegisters()
	{
		DWORD debugRegisterCount = 0;

		CONTEXT ctx;
		ZeroMemory(&ctx, sizeof(CONTEXT)); 
		ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS; 

		if(GetThreadContext(GetCurrentThread(), &ctx) == 0)
			return 4;

		if(ctx.Dr0 != 0)
			++debugRegisterCount;
		if(ctx.Dr1 != 0)
			++debugRegisterCount;
		if(ctx.Dr2 != 0)
			++debugRegisterCount;
		if(ctx.Dr3 != 0)
			++debugRegisterCount;

		return (debugRegisterCount <= 0);
	}

	inline DWORD GetHeapFlags()
	{
		DWORD *HeapFlags;
		DWORD returnVal;
		OSVERSIONINFO osvi;

		#ifdef __INTRIN_H_
			#ifdef _M_IX86
				HeapFlags = (DWORD_PTR *) __readfsdword(0x30);
				HeapFlags = (DWORD_PTR *) *(HeapFlags + (0x18 / sizeof(DWORD_PTR)));
				HeapFlags = (DWORD_PTR *) *(HeapFlags + (0x10 / sizeof(DWORD_PTR)));
			#elif _M_X64
				HeapFlags = (DWORD_PTR *) __readgsqword(0x60);
				HeapFlags = (DWORD_PTR *) *(HeapFlags + (0x30 / sizeof(DWORD_PTR)));
				HeapFlags = (DWORD_PTR *) *(HeapFlags + (0x20 / sizeof(DWORD_PTR)));
			#endif
		#else
			__asm {
				MOV EAX, FS:[0x30]
				MOV EAX, [EAX+0x18]
				MOV EAX, [EAX+0x10]
				MOV [HeapFlags], EAX
			};
		#endif

		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		if(GetVersionEx(&osvi) == 0)
			return 0;

		if(osvi.dwMajorVersion >= 6)
			returnVal = (DWORD) HeapFlags <= 0x80000000;
		else
			returnVal = (DWORD) HeapFlags == 0;

		return returnVal;
	}

	inline DWORD GetNtGlobalFlags()
	{
		DWORD_PTR *NtGlobalFlags;

		#ifdef __INTRIN_H_
			#ifdef _M_IX86
				NtGlobalFlags = (DWORD_PTR *)__readfsdword(0x30);
				NtGlobalFlags = (DWORD_PTR *)*((DWORD_PTR *) NtGlobalFlags + (0x68 / sizeof(DWORD_PTR)));
			#elif _M_X64
				NtGlobalFlags = (DWORD_PTR *) __readgsqword(0x60);
				NtGlobalFlags = (DWORD_PTR *)*(NtGlobalFlags + (0xD0 / sizeof(DWORD_PTR)));
			#endif
		#else
			__asm
			{
				MOV EAX, FS:[0x30] 
				MOV EAX, [EAX+0x68]
				MOV [NtGlobalFlags], EAX
			}
		#endif

		return ((DWORD)NtGlobalFlags ^ 0x00000070);
	}

	inline bool CheckForInterceptedException()
	{
		DWORD tickCount;

		try {
			tickCount = GetTickCount();
			throw std::exception();
		} catch(std::exception&) {
			if((GetTickCount() - tickCount) <= EXCEPTION_DELTA_IN_MILLISECONDS)
				return false;
		}

		return true;
	}

public:
	inline bool IsDebuggerDetected()
	{
		if(GetHeapFlags() == 0)
			return true;

		if(GetBeingDebugged() == 0)
			return true;

		if(GetNtGlobalFlags() == 0)
			return true;

		if(CheckRemoteDebugger() == 0)
			return true;

		if(CheckIsDbgPresent() == 0)
			return true;

		if(CheckTrapFlag() == 0)
			return true;

		if(OutputDebugCheck() == 0)
			return true;

		if(CheckSeDebugPrivilege() == 0)
			return true;

		if(GetDebugRegisters() == 0)
			return true;

		if(CheckCloseHandle() == 0)
			return true;

		if(CheckDebugPort() == 0)
			return true;

		if(CheckForInterceptedException())
			return true;

		return false;
	}
};

const char* DetectDebuggers()
{
	static SiberianTiger antiDebug;

	return antiDebug.IsDebuggerDetected() ? __FUNCTION__ : NULL;
}
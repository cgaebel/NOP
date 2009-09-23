#pragma once

#pragma region OnionDefs
	#define SystemProcessInformation 5

	#ifndef _WINDOWS_

	#define TRUE 1
	#define FALSE 0

	#define WINAPI __stdcall
	#define NT_SUCCESS(status) (status >= 0)

	#define VOID void
	typedef int BOOL;
	typedef __w64 unsigned long ULONG_PTR;
	typedef ULONG_PTR DWORD_PTR;
	typedef void *HANDLE;
	typedef unsigned long DWORD, *PDWORD;
	typedef signed short WORD, *PWORD;
	typedef unsigned long ULONG, *PULONG;
	typedef long LONG, *PLONG;
	typedef short SHORT, *PSHORT;
	typedef unsigned short USHORT, *PUSHORT;
	typedef long long LONGLONG;
	typedef unsigned int UINT;
	typedef void *LPVOID, *LPCVOID, *PVOID;
	typedef char *PCHAR, CHAR, BYTE;
	typedef unsigned char *PUCHAR, UCHAR;
	typedef DWORD *PDWORD;
	typedef ULONG_PTR SIZE_T;
	typedef LONG NTSTATUS;
	typedef unsigned short USHORT, *PUSHORT;
	typedef wchar_t WCHAR;
	typedef WCHAR *PWSTR;



	typedef struct _FLOATING_SAVE_AREA {
		DWORD ControlWord;
		DWORD StatusWord;
		DWORD TagWord;
		DWORD ErrorOffset;
		DWORD ErrorSelector;
		DWORD DataOffset;
		DWORD DataSelector;
		BYTE RegisterArea[80];
		DWORD Cr0NpxState;
	} FLOATING_SAVE_AREA;


	#ifdef _M_IX86
	#define CONTEXT_i386	0x00010000

	#define CONTEXT_CONTROL		 (CONTEXT_i386 | 0x00000001L)
	#define CONTEXT_INTEGER		 (CONTEXT_i386 | 0x00000002L)
	#define CONTEXT_SEGMENTS		(CONTEXT_i386 | 0x00000004L)
	#define CONTEXT_FLOATING_POINT  (CONTEXT_i386 | 0x00000008L)
	#define CONTEXT_DEBUG_REGISTERS (CONTEXT_i386 | 0x00000010L)
	#define CONTEXT_EXTENDED_REGISTERS  (CONTEXT_i386 | 0x00000020L)

	#define CONTEXT_FULL (CONTEXT_CONTROL | CONTEXT_INTEGER |\
						  CONTEXT_SEGMENTS)

	#define CONTEXT_ALL			 (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_SEGMENTS | \
									 CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS | \
									 CONTEXT_EXTENDED_REGISTERS)
	typedef struct _CONTEXT {
		DWORD ContextFlags;

		DWORD Dr0;
		DWORD Dr1;
		DWORD Dr2;
		DWORD Dr3;
		DWORD Dr6;
		DWORD Dr7;

		FLOATING_SAVE_AREA FloatSave;

		DWORD SegGs;
		DWORD SegFs;
		DWORD SegEs;
		DWORD SegDs;

		DWORD Edi;
		DWORD Esi;
		DWORD Ebx;
		DWORD Edx;
		DWORD Ecx;
		DWORD Eax;


		DWORD Ebp;
		DWORD Eip;
		DWORD SegCs;
		DWORD EFlags;
		DWORD Esp;
		DWORD SegSs;

		BYTE	ExtendedRegisters[512];

	} CONTEXT;
	typedef double ULONGLONG;

	#elif _M_X64

	#define CONTEXT_AMD64 0x100000

	#define CONTEXT_CONTROL (CONTEXT_AMD64 | 0x1L)
	#define CONTEXT_INTEGER (CONTEXT_AMD64 | 0x2L)
	#define CONTEXT_SEGMENTS (CONTEXT_AMD64 | 0x4L)
	#define CONTEXT_FLOATING_POINT  (CONTEXT_AMD64 | 0x8L)
	#define CONTEXT_DEBUG_REGISTERS (CONTEXT_AMD64 | 0x10L)

	#define CONTEXT_FULL (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_FLOATING_POINT)

	#define CONTEXT_ALL (CONTEXT_CONTROL | CONTEXT_INTEGER | CONTEXT_SEGMENTS | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS)

	#define CONTEXT_EXCEPTION_ACTIVE 0x8000000
	#define CONTEXT_SERVICE_ACTIVE 0x10000000
	#define CONTEXT_EXCEPTION_REQUEST 0x40000000
	#define CONTEXT_EXCEPTION_REPORTING 0x80000000


	typedef struct DECLSPEC_ALIGN(16) _CONTEXT {
		DWORD64 P1Home;
		DWORD64 P2Home;
		DWORD64 P3Home;
		DWORD64 P4Home;
		DWORD64 P5Home;
		DWORD64 P6Home;

		DWORD ContextFlags;
		DWORD MxCsr;
		WORD SegCs;
		WORD SegDs;
		WORD SegEs;
		WORD SegFs;
		WORD SegGs;
		WORD SegSs;
		DWORD EFlags;

		DWORD64 Dr0;
		DWORD64 Dr1;
		DWORD64 Dr2;
		DWORD64 Dr3;
		DWORD64 Dr6;
		DWORD64 Dr7;


		DWORD64 Rax;
		DWORD64 Rcx;
		DWORD64 Rdx;
		DWORD64 Rbx;
		DWORD64 Rsp;
		DWORD64 Rbp;
		DWORD64 Rsi;
		DWORD64 Rdi;
		DWORD64 R8;
		DWORD64 R9;
		DWORD64 R10;
		DWORD64 R11;
		DWORD64 R12;
		DWORD64 R13;
		DWORD64 R14;
		DWORD64 R15;

		DWORD64 Rip;

		union {
			XMM_SAVE_AREA32 FltSave;
			struct {
				M128A Header[2];
				M128A Legacy[8];
				M128A Xmm0;
				M128A Xmm1;
				M128A Xmm2;
				M128A Xmm3;
				M128A Xmm4;
				M128A Xmm5;
				M128A Xmm6;
				M128A Xmm7;
				M128A Xmm8;
				M128A Xmm9;
				M128A Xmm10;
				M128A Xmm11;
				M128A Xmm12;
				M128A Xmm13;
				M128A Xmm14;
				M128A Xmm15;
			};
		};

		M128A VectorRegister[26];
		DWORD64 VectorControl;


		DWORD64 DebugControl;
		DWORD64 LastBranchToRip;
		DWORD64 LastBranchFromRip;
		DWORD64 LastExceptionToRip;
		DWORD64 LastExceptionFromRip;
	} CONTEXT, *PCONTEXT;
	typedef unsigned __int64 ULONGLONG;

	#endif


	typedef union _LARGE_INTEGER {
		union {
			DWORD LowPart;
			LONG HighPart;
		};
		union {
			DWORD LowPart;
			LONG HighPart;
		} u;
		LONGLONG QuadPart;
	}LARGE_INTEGER, *PLARGE_INTEGER;

	typedef struct _IO_COUNTERS {
		ULONGLONG ReadOperationCount;
		ULONGLONG WriteOperationCount;
		ULONGLONG OtherOperationCount;
		ULONGLONG ReadTransferCount;
		ULONGLONG WriteTransferCount;
		ULONGLONG OtherTransferCount;
	}IO_COUNTERS, *PIO_COUNTERS;


	typedef struct _LIST_ENTRY {
		struct _LIST_ENTRY  *Flink;
		struct _LIST_ENTRY  *Blink;
	} LIST_ENTRY, *PLIST_ENTRY;


	#endif

	typedef struct _UNICODE_STRING {
		USHORT Length;
		USHORT MaximumLength;
		PWSTR Buffer;
	} UNICODE_STRING, *PUNICODE_STRING;

	typedef struct _CLIENT_ID
	{
		PVOID UniqueProcess;
		PVOID UniqueThread;
	} CLIENT_ID, *PCLIENT_ID;

	typedef long KPRIORITY;

	typedef enum _KWAIT_REASON
	{
		Executive = 0,
		FreePage = 1,
		PageIn = 2,
		PoolAllocation = 3,
		DelayExecution = 4,
		Suspended = 5,
		UserRequest = 6,
		WrExecutive = 7,
		WrFreePage = 8,
		WrPageIn = 9,
		WrPoolAllocation = 10,
		WrDelayExecution = 11,
		WrSuspended = 12,
		WrUserRequest = 13,
		WrEventPair = 14,
		WrQueue = 15,
		WrLpcReceive = 16,
		WrLpcReply = 17,
		WrVirtualMemory = 18,
		WrPageOut = 19,
		WrRendezvous = 20,
		Spare2 = 21,
		Spare3 = 22,
		Spare4 = 23,
		Spare5 = 24,
		WrCalloutStack = 25,
		WrKernel = 26,
		WrResource = 27,
		WrPushLock = 28,
		WrMutex = 29,
		WrQuantumEnd = 30,
		WrDispatchInt = 31,
		WrPreempted = 32,
		WrYieldExecution = 33,
		WrFastMutex = 34,
		WrGuardedMutex = 35,
		WrRundown = 36,
		MaximumWaitReason = 37
	} KWAIT_REASON;

	typedef struct _VM_COUNTERS { 
		SIZE_T PeakVirtualSize; 
		SIZE_T VirtualSize; 
		ULONG PageFaultCount; 
		SIZE_T PeakWorkingSetSize; 
		SIZE_T WorkingSetSize; 
		SIZE_T QuotaPeakPagedPoolUsage; 
		SIZE_T QuotaPagedPoolUsage; 
		SIZE_T QuotaPeakNonPagedPoolUsage; 
		SIZE_T QuotaNonPagedPoolUsage; 
		SIZE_T PagefileUsage; 
		SIZE_T PeakPagefileUsage; 
	} VM_COUNTERS; 


	typedef struct _SYSTEM_THREAD_INFORMATION {
		LARGE_INTEGER KernelTime;
		LARGE_INTEGER UserTime;
		LARGE_INTEGER CreateTime;
		ULONG WaitTime;
		PVOID StartAddress;
		CLIENT_ID ClientId;
		KPRIORITY Priority;
		KPRIORITY BasePriority;
		ULONG ContextSwitchCount;
		LONG State;
		LONG WaitReason;
	} SYSTEM_THREAD_INFORMATION, * PSYSTEM_THREAD_INFORMATION;

	typedef struct _OBJECT_ATTRIBUTES {
		ULONG Length;
		HANDLE RootDirectory;
		PUNICODE_STRING ObjectName;
		ULONG Attributes;
		PVOID SecurityDescriptor;
		PVOID SecurityQualityOfService;
	} OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

	typedef struct _SYSTEM_PROCESS_INFORMATION {
		ULONG NextEntryOffset;
		ULONG ThreadCount;
		ULONG Reserved1[6];
		LARGE_INTEGER CreateTime;
		LARGE_INTEGER UserTime;
		LARGE_INTEGER KernelTime;
		UNICODE_STRING ProcessName;
		KPRIORITY BasePriority;
		ULONG ProcessId;
		ULONG InheritedFromProcessId;
		ULONG HandleCount;
		ULONG Reserved2[2];
		VM_COUNTERS VmCounters;
		IO_COUNTERS IoCounters;
		SYSTEM_THREAD_INFORMATION Threads[1];
	} SYSTEM_PROCESS_INFORMATION, * PSYSTEM_PROCESS_INFORMATION;

	typedef struct _PEB_LDR_DATA
	{
		ULONG Length;
		BOOL Initialized;
		PVOID SsHandle;
		LIST_ENTRY InLoadOrderModuleList;
		LIST_ENTRY InMemoryOrderModuleList;
		LIST_ENTRY InInitializationOrderModuleList;
	} PEB_LDR_DATA, *PPEB_LDR_DATA;

	typedef struct _LDR_MODULE {
		LIST_ENTRY InLoadOrderModuleList;
		LIST_ENTRY InMemoryOrderModuleList;
		LIST_ENTRY InInitializationOrderModuleList;
		PVOID BaseAddress;
		PVOID EntryPoint;
		ULONG SizeOfImage;
		UNICODE_STRING FullDllName;
		UNICODE_STRING BaseDllName;
		ULONG Flags;
		SHORT LoadCount;
		SHORT TlsIndex;
		LIST_ENTRY HashTableEntry;
		ULONG TimeDateStamp;
	} LDR_MODULE, *PLDR_MODULE;

	typedef enum _OBJECT_INFORMATION_CLASS{
		ObjectBasicInformation,
		ObjectNameInformation,
		ObjectTypeInformation,
		ObjectAllTypesInformation,
		ObjectHandleInformation
	} OBJECT_INFORMATION_CLASS;

	typedef DWORD ACCESS_MASK;

	typedef struct _PUBLIC_OBJECT_BASIC_INFORMATION {
		ULONG Attributes;
		ACCESS_MASK GrantedAccess;
		ULONG HandleCount;
		ULONG PointerCount;
		ULONG Reserved[10];
	} PUBLIC_OBJECT_BASIC_INFORMATION, *PPUBLIC_OBJECT_BASIC_INFORMATION;
#pragma endregion

DWORD GetThreadStartAddress(HANDLE ThreadHandle);
DWORD EnumerateThreads(DWORD ProcessId);
DWORD EnumerateFunctions();

#include "HideThreadFromDebugger.h"
#include "NTInternals.h"

typedef NTSTATUS (*NSIT)(HANDLE, UINT, PVOID, ULONG);

static NSIT NtSetInformationThread = (NSIT)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtSetInformationThread");

void HideThreadFromDebugger(HANDLE toHide)
{
	NtSetInformationThread(toHide, 0x11, 0, 0);
}

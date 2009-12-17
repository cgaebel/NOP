#include "Core.h"
#include "NTInternals.h"

static TEB* GetTEB()
{
	__asm mov eax, fs:[0x18]
	__asm ret
}

// check if the "signature" variable is inside this module, if so it is our module
static bool PointerIsInModule(const void* pointer, const LDR_MODULE* module)
{
	return	(
				(DWORD)pointer > (DWORD)(module->BaseAddress)
			)
			&&
			(
				(DWORD)pointer <
				(
					(DWORD)(module->BaseAddress) + (DWORD)(module->SizeOfImage)
				)
			);
}

static void RemoveFromRootList(LIST_ENTRY* toRemove)
{
	toRemove->Blink->Flink = toRemove->Flink;
	toRemove->Flink->Blink = toRemove->Blink;
}

static void RemoveThisModuleFromList(LIST_ENTRY* listHead)
{
	// Can point to any string as long as it's in OUR address space.
	static const char* signature = __DATE__;

	for(LIST_ENTRY* currentEntry = listHead->Flink; currentEntry != listHead; currentEntry = currentEntry->Flink)
        if(PointerIsInModule(signature, CONTAINING_RECORD(currentEntry, LDR_MODULE, InMemoryOrderModuleList)))
			RemoveFromRootList(currentEntry);
}

PASSIVE_PROTECTION(HideFromPEB, "Hiding the module...")
{
    PEB_LDR_DATA* loader = GetTEB()->Peb->LoaderData;

	RemoveThisModuleFromList(&(loader->InInitializationOrderModuleList));
	RemoveThisModuleFromList(&(loader->InLoadOrderModuleList));
	RemoveThisModuleFromList(&(loader->InMemoryOrderModuleList));

	return NO_HACK_DETECTED;
}

#include "Core.h"
#include "NTInternals.h"

static TEB* GetTEB()
{
	__asm mov eax, fs:[0x18]
	__asm ret
}

// check if the "signature" variable is inside this module, if so it is our module
static bool PointerIsInModule(DWORD pointer, const LDR_MODULE* module)
{
	DWORD startAddress = (DWORD)module->BaseAddress;
	DWORD endAddress = startAddress + (DWORD)module->SizeOfImage;

	return (pointer > startAddress) && (pointer < endAddress);
}

// Removes the list entry from the list is belongs to.
static void RemoveFromRootList(LIST_ENTRY* toRemove)
{
	toRemove->Blink->Flink = toRemove->Flink;
	toRemove->Flink->Blink = toRemove->Blink;
}

// Removes the current dll from the list it is passed.
static void RemoveThisModuleFromList(LIST_ENTRY* listHead)
{
	// Can point to any string as long as it's in OUR address space.
	static DWORD signature = (DWORD)__DATE__;

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

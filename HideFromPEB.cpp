#include "Core.h"
#include "NTInternals.h"

PASSIVE_PROTECTION(HideFromPEB, "Hiding the module...")
{
	TEB* threadEntryBlock;

	__asm
	{
		mov eax, fs:[0x18]
		mov threadEntryBlock, eax
	}

	LDR_MODULE* pLM = NULL;

	auto pSig = __DATE__;	// Can point to any string.

    auto* pLDR = threadEntryBlock->Peb->LoaderData;  // get a pointer to the loader data structure within the PEB (process environment block) within the TEB

    auto* pMark = &(pLDR->InMemoryOrderModuleList);  // the list is circular-linked, so we have to mark the point at which we start traversing it so we know when we've made a full traversal

	auto* pEntry = pMark->Flink;
    for(; pEntry != pMark; pEntry = pEntry->Flink)
    {
        pLM = CONTAINING_RECORD(pEntry, LDR_MODULE, InMemoryOrderModuleList);  // CONTAINING_RECORD is in the DDK, it basically just gets a pointer to the actual structure from the linked list element
        if((DWORD)pSig > (DWORD)pLM->BaseAddress && (DWORD)pSig < ((DWORD)pLM->BaseAddress + (DWORD)pLM->SizeOfImage))  // check if the "signature" variable is inside this module, if so it is our module
        {
            pEntry->Blink->Flink = pEntry->Flink;
            pEntry->Flink->Blink = pEntry->Blink;
        }
    }

    pMark = &(pLDR->InLoadOrderModuleList);

    for(pEntry = pMark->Flink; pEntry != pMark; pEntry = pEntry->Flink)
    {
        pLM = CONTAINING_RECORD(pEntry, LDR_MODULE, InLoadOrderModuleList);
        if((DWORD)pSig > (DWORD)pLM->BaseAddress && (DWORD)pSig < ((DWORD)pLM->BaseAddress + (DWORD)pLM->SizeOfImage))
        {
            pEntry->Blink->Flink = pEntry->Flink;
            pEntry->Flink->Blink = pEntry->Blink;
        }
    }

    pMark = &(pLDR->InInitializationOrderModuleList);

    for(pEntry = pMark->Flink; pEntry != pMark; pEntry = pEntry->Flink)
    {
        pLM = CONTAINING_RECORD(pEntry, LDR_MODULE, InInitializationOrderModuleList);
        if((DWORD)pSig > (DWORD)pLM->BaseAddress && (DWORD)pSig < ((DWORD)pLM->BaseAddress + (DWORD)pLM->SizeOfImage))
        {
            pEntry->Blink->Flink = pEntry->Flink;
            pEntry->Flink->Blink = pEntry->Blink;
        }
    }

	return NO_HACK_DETECTED;
}

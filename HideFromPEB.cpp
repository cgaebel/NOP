#include "CProtectionManager.h"
#include "NTInternals.h"

static PTEB GetCurrentTeb()
{
	PTEB pTeb = NULL;

	__asm
	{
		mov eax,fs:[0x18]
		mov pTeb , eax
	}

	return (PTEB)pTeb;
}

// Source: https://forum.gamedeception.net/showthread.php?t=4091
static void RemoveModuleFromPEB(PTEB threadEntryBlock)
{
    PPEB_LDR_DATA pLDR;
    PLIST_ENTRY pMark, pEntry;
    PLDR_MODULE pLM;
    char* pSig = "unrelated text";	// anything can go here this string does not matter in fact you could use a pointer to one of the existing variables I just wanted to make it obvious what was going on.

    pLDR = threadEntryBlock->Peb->LoaderData;  // get a pointer to the loader data structure within the PEB (process environment block) within the TEB

    pMark = &(pLDR->InMemoryOrderModuleList);  // the list is circular-linked, so we have to mark the point at which we start traversing it so we know when we've made a full traversal

    for(pEntry = pMark->Flink; pEntry != pMark; pEntry = pEntry->Flink)
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
}

const char* HideFromPEB()
{
	RemoveModuleFromPEB(GetCurrentTeb());

	return NULL;
}

#include "CProtectionManager.h"
#include "NOP.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Hides the module.
/// 
/// @author	Brian
/// @date	Unknown
/// 
/// @param	hModule	The module to hide.
////////////////////////////////////////////////////////////////////////////////////////////////////
static void HideModule(HINSTANCE hModule)
{
	DWORD dwPEB_LDR_DATA = 0;

	__asm
	{
		pushad
		pushfd

		mov eax, fs:[0x30]
		mov eax, [eax + 0xC]
		mov dwPEB_LDR_DATA, eax

		mov esi, [eax + 0xC]
		mov edx, [eax + 0x10]

LoopInLoadOrderModuleList: 
		lodsd
		mov esi, eax
		mov ecx, [eax + 0x18]
		cmp ecx, hModule
		jne SkipA
		mov ebx, [eax]
		mov ecx, [eax + 0x4]
		mov [ecx], ebx
		mov [ebx + 0x4], ecx
		jmp InMemoryOrderModuleList

SkipA:
		cmp edx, esi
		jne LoopInLoadOrderModuleList

InMemoryOrderModuleList:
		mov eax, dwPEB_LDR_DATA
		mov esi, [eax + 0x14]
		mov edx, [eax + 0x18]

LoopInMemoryOrderModuleList: 
		lodsd
		mov esi, eax
		mov ecx, [eax + 0x10]
		cmp ecx, hModule
		jne SkipB
		mov ebx, [eax] 
		mov ecx, [eax + 0x4]
		mov [ecx], ebx
		mov [ebx + 0x4], ecx
		jmp InInitializationOrderModuleList

SkipB:
		cmp edx, esi
		jne LoopInMemoryOrderModuleList

InInitializationOrderModuleList:
		mov eax, dwPEB_LDR_DATA
		mov esi, [eax + 0x1C]
		mov edx, [eax + 0x20]

LoopInInitializationOrderModuleList: 
		lodsd
		mov esi, eax
		mov ecx, [eax + 0x08]
		cmp ecx, hModule
		jne SkipC
		mov ebx, [eax] 
		mov ecx, [eax + 0x4]
		mov [ecx], ebx
		mov [ebx + 0x4], ecx
		jmp Finished

SkipC:
		cmp edx, esi
		jne LoopInInitializationOrderModuleList

Finished:
		popfd
		popad
	}
}

const char* ModuleHiding()
{
	__try {
		HideModule(g_hInstance);
	} __except(EXCEPTION_EXECUTE_HANDLER) {
		return __FUNCTION__;
	}
	return NULL;
}

#include "CProtectionManager.h"
#include "Patching.h"

/*
DEF_CALL(0x0057170D);
DEF_CALL(0x004C3B60);
DEF_CALL(0x005CEFAF);
DEF_CALL(0x00505DD0);
DEF_CALL(0x00507C00);
DEF_CALL(0x004C3000);
DEF_CALL(0x0057112E);

#pragma warning(push)
#pragma warning(disable:4733)
__declspec(naked) static void __cdecl ZPostConnect(const char*, unsigned int)
{
	__asm
	{
		PUSH -1
		PUSH 0x005D6E1E
		MOV EAX,DWORD PTR FS:[0]
		PUSH EAX
		MOV DWORD PTR FS:[0],ESP
		SUB ESP,0x108
		MOV EAX,DWORD PTR DS:[0x6569F0]
		MOV ECX,DWORD PTR SS:[ESP+0x118]
		PUSH ESI
		MOV DWORD PTR SS:[ESP+0x108],EAX
		MOV EAX,DWORD PTR SS:[ESP+0x120]
		PUSH EAX
		PUSH ECX
		LEA EDX,[ESP+0x10]
		PUSH 0x5EA8E0
		PUSH EDX
		CALL_FROM_TABLE(0x0057170D)
		PUSH 0x12E
		CALL_FROM_TABLE(0x004C3B60)
		PUSH 0x0C
		MOV ESI,EAX
		CALL_FROM_TABLE(0x005CEFAF)
		ADD ESP,0x18
		MOV DWORD PTR SS:[ESP+4],EAX
		TEST EAX,EAX
		MOV DWORD PTR SS:[ESP+0x114],0
		je SKIP_CALL
		LEA ECX,[ESP+8]
		PUSH ECX
		MOV ECX,EAX
		CALL_FROM_TABLE(0x00505DD0)
		JMP SKIP_EAX_CLEAR
SKIP_CALL:
		XOR EAX,EAX
SKIP_EAX_CLEAR:
		PUSH EAX
		MOV ECX,ESI
		MOV DWORD PTR SS:[ESP+0x118],-1
		CALL_FROM_TABLE(0x00507C00)
		PUSH ESI
		CALL_FROM_TABLE(0x004C3000)
		MOV ECX,DWORD PTR SS:[ESP+0x110]
		ADD ESP,4
		MOV DWORD PTR FS:[0],ECX
		MOV ECX,DWORD PTR SS:[ESP+0x108]
		POP ESI
		CALL_FROM_TABLE(0x0057112E)
		ADD ESP,0x114
		RETN
	}
}
#pragma warning(pop)
*/

//using namespace AsmJit;

//static AsmJit::Assembler ZPostConnectBuilder;

//void BuildZPostConnect()
//{
//	ZPostConnectBuilder.push(imm(-1));
//	ZPostConnectBuilder.push(imm(0x005D6E1E));
//}

const char* RestoreZPostConnect()
{
	Patching::Patch((void*)0x004325B0,
		"\x6A\xFF\x68\x1E\x6E\x5D\x00\x64\xA1\x00\x00\x00\x00\x50\x64\x89"
		"\x25\x00\x00\x00\x00\x81\xEC\x08\x01\x00\x00\xA1\xF0\x69\x65\x00"
		"\x8B\x8C\x24\x18\x01\x00\x00\x56\x89\x84\x24\x08\x01\x00\x00\x8B"
		"\x84\x24\x20\x01\x00\x00\x50\x51\x8D\x54\x24\x10\x68\xE0\xA8\x5E"
		"\x00\x52\xE8\x16\xF1\x13\x00\x68\x2E\x01\x00\x00\xE8\x5F\x15\x09"
		"\x00\x6A\x0C\x8B\xF0\xE8\xA5\xC9\x19\x00\x83\xC4\x18\x89\x44\x24"
		"\x04\x85\xC0\xC7\x84\x24\x14\x01\x00\x00\x00\x00\x00\x00\x74\x0E"
		"\x8D\x4C\x24\x08\x51\x8B\xC8\xE8\xA4\x37\x0D\x00\xEB\x02\x33\xC0"
		"\x50\x8B\xCE\xC7\x84\x24\x18\x01\x00\x00\xFF\xFF\xFF\xFF\xE8\xBD"
		"\x55\x0D\x00\x56\xE8\xB7\x09\x09\x00\x8B\x8C\x24\x10\x01\x00\x00"
		"\x83\xC4\x04\x64\x89\x0D\x00\x00\x00\x00\x8B\x8C\x24\x08\x01\x00"
		"\x00\x5E\xE8\xC7\xEA\x13\x00\x81\xC4\x14\x01\x00\x00\xC3"
	, 190);

	//BuildZPostConnect();
	//Patching::PatchUnconditionalJump(0x004325B0, (DWORD)ZPostConnectBuilder.make());
	return NULL;
}

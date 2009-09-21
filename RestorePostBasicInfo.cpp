#include "CProtectionManager.h"
#include "Patching.h"

DEF_CALL(0x00571698);
DEF_CALL(0x004ABCC0);
DEF_CALL(0x004C3B60);
DEF_CALL(0x005CEFAF);
DEF_CALL(0x00506030);
DEF_CALL(0x00507C00);
DEF_CALL(0x004C3000);

#pragma warning(push)
#pragma warning(disable:4733)
__declspec(naked) static void PostBasicInfo()
{
	__asm
	{
		MOV EAX,DWORD PTR FS:[0]
		PUSH -1
		PUSH 0x005DA20B
		PUSH EAX
		MOV DWORD PTR FS:[0],ESP
		SUB ESP,0x20
		PUSH ESI
		PUSH EDI
		MOV ESI,ECX
		CALL DWORD PTR DS:[0x005E655C]
		MOV ECX,DWORD PTR DS:[ESI+0x50]
		MOV EDI,EAX
		MOV AL,BYTE PTR DS:[ECX+0x31]
		TEST AL,AL
		JE EPILOGUE
		MOV EAX,DWORD PTR DS:[ECX]
		CALL DWORD PTR DS:[EAX+0x3C]
		TEST AL,AL
		JE SKIP_BUILD_REGISTERS
		MOV EAX,DWORD PTR DS:[ESI+0x30]
		MOV ECX,DWORD PTR DS:[EAX+0x18]
		MOV EDX,DWORD PTR DS:[EAX+0x0C]
		MOV EAX,DWORD PTR DS:[ECX*4+EDX]
		MOV EAX,DWORD PTR DS:[ESI+0x50]
		FSUB DWORD PTR DS:[EAX+0x5A8]
		FLD DWORD PTR DS:[EAX]
		FCOMP DWORD PTR DS:[0x5E7580]
		FSTSW AX
		TEST AH,0x41
		JE EPILOGUE
SKIP_BUILD_REGISTERS:
		CALL_FROM_TABLE(0x004ABCC0)
		MOV EDX,DWORD PTR DS:[ESI+0x38]
		MOV ECX,EDI
		SUB ECX,EDX
		CMP ECX,64
		JL EPILOGUE
		MOV EAX,DWORD PTR DS:[ESI+0x30]
		MOV DWORD PTR DS:[ESI+0x38],EDI
		MOV EDX,DWORD PTR DS:[EAX+0x18]
		MOV EAX,DWORD PTR DS:[EAX+0x0C]
		MOV EAX,DWORD PTR DS:[EDX*4+EAX]
		MOV ECX,DWORD PTR DS:[EAX]
		MOV ESI,DWORD PTR DS:[ESI+0x50]
		MOV DWORD PTR SS:[ESP+0x0C],ECX
		MOV EAX,DWORD PTR DS:[ESI+0x58]
		TEST EAX,EAX
		JNE SKIP_SECTION_1
		LEA EAX,[ESI+0x4C]
		JMP SKIP_SECTION_2
SKIP_SECTION_1:
		MOV EDX,DWORD PTR DS:[EAX+0x20]
		MOV EAX,DWORD PTR DS:[EAX+0x0C]
		MOV EAX,DWORD PTR DS:[EDX*4+EAX]
SKIP_SECTION_2:
		FLD DWORD PTR DS:[EAX]
		CALL_FROM_TABLE(0x00571698)
		MOV WORD PTR SS:[ESP+0x10],AX
		MOV EAX,DWORD PTR DS:[ESI+0x58]
		TEST EAX,EAX
		JNE SKIP_SECTION_3
		LEA EAX,[ESI+0x4C]
		JMP SKIP_SECTION_4
SKIP_SECTION_3:
		MOV ECX,DWORD PTR DS:[EAX+0x20]
		MOV EDX,DWORD PTR DS:[EAX+0x0C]
		MOV EAX,DWORD PTR DS:[ECX*4+EDX]
SKIP_SECTION_4:
		FLD DWORD PTR DS:[EAX+4]
		CALL_FROM_TABLE(0x00571698)
		MOV WORD PTR SS:[ESP+0x12],AX
		MOV EAX,DWORD PTR DS:[ESI+0x58]
		TEST EAX,EAX
		JNE SKIP_SECTION_5
		LEA EAX,[ESI+0x4C]
		JMP SKIP_SECTION_6
SKIP_SECTION_5:
		MOV ECX,DWORD PTR DS:[EAX+0x20]
		MOV EDX,DWORD PTR DS:[EAX+0x0C]
		MOV EAX,DWORD PTR DS:[ECX*4+EDX]
SKIP_SECTION_6:
		FLD DWORD PTR DS:[EAX+8]
		CALL_FROM_TABLE(0x00571698)
		MOV WORD PTR SS:[ESP+0x14],AX
		MOV EAX,DWORD PTR DS:[ESI+0x68]
		FLD DWORD PTR DS:[EAX+0x40]
		CALL_FROM_TABLE(0x00571698)
		MOV WORD PTR SS:[ESP+0x16],AX
		MOV ECX,DWORD PTR DS:[ESI+0x68]
		FLD DWORD PTR DS:[ECX+0x44]
		CALL_FROM_TABLE(0x00571698)
		MOV WORD PTR SS:[ESP+0x18],AX
		MOV EDX,DWORD PTR DS:[ESI+0x68]
		FLD DWORD PTR DS:[EDX+0x48]
		CALL_FROM_TABLE(0x00571698)
		MOV WORD PTR SS:[ESP+0x1A],AX
		FLD DWORD PTR DS:[ESI+0x500]
		FMUL DWORD PTR DS:[0x5F0738]
		CALL_FROM_TABLE(0x00571698)
		MOV WORD PTR SS:[ESP+0x1C],AX
		FLD DWORD PTR DS:[ESI+0x504]
		FMUL DWORD PTR DS:[0x5F0738]
		CALL_FROM_TABLE(0x00571698)
		MOV WORD PTR SS:[ESP+0x1E],AX
		FLD DWORD PTR DS:[ESI+0x508]
		FMUL DWORD PTR DS:[0x5F0738]
		CALL_FROM_TABLE(0x00571698)
		MOV WORD PTR SS:[ESP+0x20],AX
		MOV EAX,DWORD PTR DS:[ESI+0x548]
		MOV BYTE PTR SS:[ESP+0x22],AL
		MOV EAX,DWORD PTR DS:[ESI+0x54C]
		MOV BYTE PTR SS:[ESP+0x23],AL
		MOV EAX,DWORD PTR DS:[ESI+0x2E0]
		PUSH 0x271C
		MOV BYTE PTR SS:[ESP+0x28],AL
		CALL_FROM_TABLE(0x004C3B60)
		PUSH 0x10
		MOV ESI,EAX
		CALL_FROM_TABLE(0x005CEFAF)
		ADD ESP,8
		MOV DWORD PTR SS:[ESP+8],EAX
		TEST EAX,EAX
		MOV DWORD PTR SS:[ESP+0x30],0
		JE CLEAR_EAX
		PUSH 0x19
		LEA ECX,[ESP+0x10]
		PUSH ECX
		MOV ECX,EAX
		CALL_FROM_TABLE(0x00506030)
		JMP SKIP_CLEAR_EAX
CLEAR_EAX:
		XOR EAX,EAX
SKIP_CLEAR_EAX:
		PUSH EAX
		MOV ECX,ESI
		MOV DWORD PTR SS:[ESP+0x34],-1
		CALL_FROM_TABLE(0x00507C00)
		PUSH ESI
		CALL_FROM_TABLE(0x004C3000)
		ADD ESP,4
EPILOGUE:
		MOV ECX,DWORD PTR SS:[ESP+0x28]
		POP EDI
		POP ESI
		MOV DWORD PTR FS:[0],ECX
		ADD ESP,0x2C
		RETN
	}
}
#pragma warning(pop)

const char* RestorePostBasicInfo()
{
	Patching::PatchUnconditionalJump(0x004A1BF0, (DWORD)PostBasicInfo);

	return NULL;
}
/*****************************
*   Universal Detour Class   *
*            v2.0            *
******************************
*       By LanceVorgin       *
******************************
*        UNI owns all        *
*****************************/

/*
All credit for CDetourDis goes to Micro$oft
GG CB for the ByteArray idea - asshole :P

License:
 I, LanceVorgin, allow you to use these classes in any of your projects under the following conditions:
   * My name appears in your readme and credits along with the fact that my CDetour was used
   * You do not take credit for CDetour
That's all. GPL, closed source, private, it's all good :)

Detour Settings:
	bAutoReturn - If true, after hookfunc is called, the original function will be called with the original args - unless Ret(false) has been called
	bNoRegs - If true, detour does not modify any registers besides esp. Use for fastcalls / other funcs that take params in regs (evil msvc 2k5)
				(usually, detour will save ecx and reset it in Org for class funcs)
	bPolymorphic - If false the detour is a jmp. If true the detour is randomized and impliments a random number. These are alot bigger than 5 bytes
	iArgsOverride - Usually, the hookfunc is defined as identical to the function hooked in both args and calling convention. With this set, the
				stack will be modified after the hookfunc is called as if an stdcall with x args has been called. Useful if you have a stdcall
				func with like 10 args that you don't give a shit about (you'd otherwise have to have 10 junk args on your hookfunc)
	iBytesToOverwrite - Overrides the automatic reassembler.

Todo:
 * Add setproxy

 * Add Polymorph Dimension: replace push

   8D 64 24 FC          lea         esp,[esp-4]
   C7 04 24 78 56 34 12 mov         dword ptr [esp], 12345678h

   83 EC 04             sub         esp,4
   C7 04 24 78 56 34 12 mov         dword ptr [esp], 12345678h
*/

#include "../Patching.h"
#include "CDetourDis.h"
#include "CDetour.h"

using namespace Patching;

void* CDetour::NullOrgFunc( ... )
{
	return NULL;
}

CDetourOrg_Func CDetour::NullOrg = (CDetourOrg_Func)CDetour::NullOrgFunc;

int CDetour::g_iApplied = 0;
int CDetour::g_iRemoved = 0;

int CDetour::Applied()
{
	return g_iApplied;
}

int CDetour::Removed()
{
	return g_iRemoved;
}

void CDetour::InitRand()
{
	srand(GetTickCount());
}

CDetour::CDetour()
{
	m_pDetourBuf = NULL;
	m_pOrgBuf = NULL;
	m_pGateBuf = NULL;
	m_pOrgGateBuf = NULL;

	m_bApplied = false;

	Deconstruct();
};

CDetour::~CDetour()
{
	Deconstruct();
};

void CDetour::Deconstruct()
{
	m_bReady = false;

	if(m_bApplied)
		Remove();

	FreeBuffers();

	Org = NullOrgFunc;

	m_bAutoReturn = false;
	m_bNoRegs = false;
	m_bPolymorphic = false;
	m_iArgsOverride = -1;
	m_iBytesToOverwrite = -1;

	m_pAddr = NULL;
	m_pFuncToCall = NULL;

	m_ubDetourType = 0;
	m_iDetourSize = 0;
	m_pOrgAfterDetour = NULL;

	m_bDontReplaceOrgClassInstance = false;

	m_bReturnToOriginal = false;
	m_pGateStack = NULL;
	m_pGateRealRet = NULL;
	m_pAutoReturnEndStack = NULL;

	m_pRetAddress = NULL;
	m_pClassInstance = NULL;
	m_pOrgReturnAddress = NULL;
	m_pOrgStack = NULL;
}

void CDetour::FreeBuffers()
{
	m_bReady = false;

	if(m_pDetourBuf)
		free(m_pDetourBuf);

	if(m_pOrgBuf)
		free(m_pOrgBuf);

	if(m_pGateBuf)
		free(m_pGateBuf);

	if(m_pOrgGateBuf)
		free(m_pOrgGateBuf);

	m_pDetourBuf = NULL;
	m_pOrgBuf = NULL;
	m_pGateBuf = NULL;
	m_pOrgGateBuf = NULL;
}

bool CDetour::Detour(BYTE* pAddr, BYTE* pFuncToCall, bool bAutoReturn, bool bNoRegs, bool bPolymorphic, int iArgsOverride, int iBytesToOverwrite)
{
	Deconstruct();

	if(!pAddr || !pFuncToCall)
		return false;

	m_pAddr = pAddr;
	m_pFuncToCall = pFuncToCall;

	m_bAutoReturn = bAutoReturn;
	m_bNoRegs = bNoRegs;
	m_bPolymorphic = bPolymorphic;
	m_iArgsOverride = iArgsOverride;
	m_iBytesToOverwrite = iBytesToOverwrite;

	return Generate();
}

bool CDetour::Detour(LPCSTR lpLibraryName, LPCSTR lpProcName, BYTE* pFuncToCall, bool bAutoReturn, bool bNoRegs, bool bPolymorphic, int iArgsOverride, int iBytesToOverwrite)
{
	HMODULE hModule = LoadLibraryA(lpLibraryName);

	if(!hModule)
		return false;

	BYTE* pTargetAddress = (BYTE*)GetProcAddress(hModule, lpProcName);

	if(!pTargetAddress)
		return false;

	return Detour(pTargetAddress, pFuncToCall, bAutoReturn, bNoRegs, bPolymorphic, iArgsOverride, iBytesToOverwrite);
}

bool CDetour::WriteToAddr(BYTE* pBuffer, int iSize)
{
	if(!m_bReady)
		return false;

	Patch(m_pAddr, pBuffer, iSize);

	return true;
}

bool CDetour::Apply()
{
	if(m_bApplied)
		return false;

	if(!WriteToAddr(m_pDetourBuf, m_iDetourSize))
		return false;

	m_bApplied = true;

	g_iApplied++;

	return true;
}

bool CDetour::Remove()
{
	if(!m_bApplied)
		return false;

	if(!WriteToAddr(m_pOrgBuf, m_iDetourSize))
		return false;

	m_bApplied = false;

	g_iRemoved++;

	return true;
}

bool CDetour::IsReady()
{
	return m_bReady;
}

bool CDetour::IsApplied()
{
	return m_bApplied;
}

BYTE* CDetour::GetAddr()
{
	return m_pAddr;
}

BYTE* CDetour::GetFuncToCall()
{
	return m_pFuncToCall;
}

void CDetour::SetFuncToCall(BYTE* pFuncToCall)
{
	m_pFuncToCall = pFuncToCall;
}

BYTE* CDetour::GetRetAddress()
{
	return m_pRetAddress;
}

BYTE* CDetour::GetGateRetAddress()
{
	return m_pGateRealRet;
}

void CDetour::SetGateRetAddress(BYTE* pGateRetAddress)
{
	m_pGateRealRet = pGateRetAddress;
}

BYTE* CDetour::GetThisPtr()
{
	return m_pClassInstance;
}

void CDetour::SetThisPtr(BYTE* pThisPtr)
{
	m_pClassInstance = pThisPtr;
}

void CDetour::NoSetThisPtr(bool bNoSetThisPtr)
{
	m_bDontReplaceOrgClassInstance = bNoSetThisPtr;
}

void CDetour::Ret(bool bReturnToOriginal)
{
	m_bReturnToOriginal = bReturnToOriginal;
}

int CDetour::GetDetourSize()
{
	LVCByteArray Buffer;

	if(!GenerateDetour(Buffer, 0, 0))
		return -1;

	return Buffer.Size();
}

#define RAND_DETOUR_TYPES 9

bool CDetour::GenerateDetour(LVCByteArray& Buffer, BYTE* pBase, BYTE* pTarget, int iFinalSize)
{
	Buffer.Clear();

	if(m_ubDetourType > RAND_DETOUR_TYPES)
		return false;

	DWORD dwTmpRnd = ((m_ubDetourType != 0) ? (rand() | (rand() << 16)) : 0);

	switch(m_ubDetourType)
	{
	case 0:
		Buffer += (BYTE)0xE9; //jmp

		Buffer += (DWORD)(pTarget - pBase - 5);

		break;

	case 1:
	case 2:
	case 3:
		Buffer += (BYTE)0x68; //push
		Buffer += (DWORD)dwTmpRnd;

		Buffer += (BYTE)0x81; //xor dword ptr [esp]
		Buffer += (BYTE)0x34;
		Buffer += (BYTE)0x24;

		Buffer += (DWORD)((DWORD)pTarget ^ dwTmpRnd);

		break;

	case 4:
	case 5:
	case 6:
		Buffer += (BYTE)0x68; //push
		Buffer += (DWORD)(((DWORD)pTarget << ((BYTE)dwTmpRnd & 31)) | ((DWORD)pTarget >> (32 - ((BYTE)dwTmpRnd & 31))));

		Buffer += (BYTE)0xC1; //ror dword ptr [esp],
		Buffer += (BYTE)0x0C;
		Buffer += (BYTE)0x24;
		Buffer += (BYTE)dwTmpRnd;

		break;

	case 7:
	case 8:
	case 9:
		Buffer += (BYTE)0x68; //push
		Buffer += (DWORD)(pTarget - dwTmpRnd);

		Buffer += (BYTE)0x81; //add dword ptr [esp],
		Buffer += (BYTE)0x04;
		Buffer += (BYTE)0x24;

		Buffer += (DWORD)dwTmpRnd;

		break;
	}

	switch(m_ubDetourType){
	case 1:
	case 4:
	case 7:
		Buffer += (BYTE)0xC3; //ret

		break;

	case 2:
	case 5:
	case 8:
		Buffer += (BYTE)0xC2; //retn
		Buffer += (WORD)0;

		break;

	case 3:
	case 6:
	case 9:
		Buffer += (BYTE)0x83; //add esp, 4
		Buffer += (BYTE)0xC4;
		Buffer += (BYTE)0x04;

		Buffer += (BYTE)0xFF; //jmp dword ptr [esp-4]
		Buffer += (BYTE)0x64;
		Buffer += (BYTE)0x24;
		Buffer += (BYTE)0xFC;

		break;
	}

	if(iFinalSize != -1)
	{
		if(iFinalSize < (int)Buffer.Size())
			return false;

		while((int)Buffer.Size() < iFinalSize)
			Buffer += (BYTE)OP_NOP;
	}

	return true;
}

bool CDetour::Generate()
{
	FreeBuffers();

	LVCByteArray Buffer;

	if(m_bPolymorphic)
		m_ubDetourType = (BYTE)(rand() % RAND_DETOUR_TYPES) + 1;
	else
		m_ubDetourType = 0;

	int iRawDetourSize = GetDetourSize();

	if(iRawDetourSize == -1)
		return false;

	if(m_iBytesToOverwrite > 0)
		if(m_iBytesToOverwrite < iRawDetourSize)
		{
			if(!m_bPolymorphic)
				return false;

			for(m_ubDetourType = 1; m_ubDetourType <= RAND_DETOUR_TYPES; m_ubDetourType++)
			{
				iRawDetourSize = GetDetourSize();

				if(iRawDetourSize <= m_iBytesToOverwrite)
					break;
			}

			if(m_ubDetourType > RAND_DETOUR_TYPES)
				return false;
		}

	Buffer.Clear();

	if(!m_bNoRegs)
	{
		Buffer += (BYTE)0x89; //mov dword ptr, ecx
		Buffer += (BYTE)0x0D;
		Buffer += (DWORD)&m_pClassInstance;
	}

	Buffer += (BYTE)0x8F; //pop dword ptr
	Buffer += (BYTE)0x05;
	Buffer += (DWORD)&m_pRetAddress;

	Buffer += (BYTE)0x83; //sub esp, 4
	Buffer += (BYTE)0xEC;
	Buffer += (BYTE)0x04;

	int iCallOrgEndOffsetIndex = -1;
	int iCallOrgEndOffset = -1;

	if(m_bAutoReturn)
	{
		//Buffer += (BYTE)0xCC;

		Buffer += (BYTE)0xC6; //mov byte ptr
		Buffer += (BYTE)0x05;
		Buffer += (DWORD)&m_bReturnToOriginal;
		Buffer += (BYTE)1;

		Buffer += (BYTE)0x8F; //pop dword ptr
		Buffer += (BYTE)0x05;
		Buffer += (DWORD)&m_pGateRealRet;

		Buffer += (BYTE)0x89; //mov dword ptr, esp
		Buffer += (BYTE)0x25;
		Buffer += (DWORD)&m_pGateStack;

		Buffer += (BYTE)0xFF; //call dword ptr
		Buffer += (BYTE)0x15;
		Buffer += (DWORD)&m_pFuncToCall;

		Buffer += (BYTE)0x80; //cmp byte ptr
		Buffer += (BYTE)0x3D;
		Buffer += (DWORD)&m_bReturnToOriginal;
		Buffer += (BYTE)0;

		Buffer += (BYTE)0x74; //je
		iCallOrgEndOffsetIndex = Buffer + (BYTE)0;

		if(m_iArgsOverride <= 0)
		{
			Buffer += (BYTE)0x89; //mov dword ptr, esp
			Buffer += (BYTE)0x25;
			Buffer += (DWORD)&m_pAutoReturnEndStack;
		}

		Buffer += (BYTE)0x8B; //mov esp, dword ptr
		Buffer += (BYTE)0x25;
		Buffer += (DWORD)&m_pGateStack;

		Buffer += (BYTE)0xFF; //call dword ptr
		Buffer += (BYTE)0x15;
		Buffer += (DWORD)&m_pOrgGateBuf;

		if(m_iArgsOverride > 0)
		{
			iCallOrgEndOffset = Buffer.Peek() - iCallOrgEndOffsetIndex - 1;

			Buffer += (BYTE)0x8B; //mov esp, dword ptr
			Buffer += (BYTE)0x25;
			Buffer += (DWORD)&m_pGateStack;

			Buffer += (BYTE)0xFF; //push dword ptr
			Buffer += (BYTE)0x35;
			Buffer += (DWORD)&m_pGateRealRet;

			Buffer += (BYTE)0xC2; //retn
			Buffer += (WORD)(m_iArgsOverride * 4);

		} else {
			Buffer += (BYTE)0x8B; //mov esp, dword ptr
			Buffer += (BYTE)0x25;
			Buffer += (DWORD)&m_pAutoReturnEndStack;

			iCallOrgEndOffset = Buffer.Peek() - iCallOrgEndOffsetIndex - 1;

			Buffer += (BYTE)0xFF; //jmp dword ptr
			Buffer += (BYTE)0x25;
			Buffer += (DWORD)&m_pGateRealRet;

		}

	}
	else if(m_iArgsOverride > 0)
	{
		Buffer += (BYTE)0x8F; //pop dword ptr
		Buffer += (BYTE)0x05;
		Buffer += (DWORD)&m_pGateRealRet;

		Buffer += (BYTE)0x89; //mov dword ptr, esp
		Buffer += (BYTE)0x25;
		Buffer += (DWORD)&m_pGateStack;

		Buffer += (BYTE)0xFF; //call dword ptr
		Buffer += (BYTE)0x15;
		Buffer += (DWORD)&m_pFuncToCall;

		Buffer += (BYTE)0x8B; //mov esp, dword ptr
		Buffer += (BYTE)0x25;
		Buffer += (DWORD)&m_pGateStack;

		Buffer += (BYTE)0xFF; //push dword ptr
		Buffer += (BYTE)0x35;
		Buffer += (DWORD)&m_pGateRealRet;

		Buffer += (BYTE)0xC2; //retn
		Buffer += (WORD)(m_iArgsOverride * 4);

	} else {
		Buffer += (BYTE)0xFF; //jmp dword ptr
		Buffer += (BYTE)0x25;
		Buffer += (DWORD)&m_pFuncToCall;

	}

	m_pGateBuf = Buffer.Copy();

	if(m_bAutoReturn)
		*(BYTE*)&m_pGateBuf[iCallOrgEndOffsetIndex] = (BYTE)iCallOrgEndOffset;

	//-----------------

	Buffer.Clear();

	Buffer += (BYTE)0x8F; //pop dword ptr
	Buffer += (BYTE)0x05;
	Buffer += (DWORD)&m_pOrgReturnAddress;

	Buffer += (BYTE)0x89; //mov dword ptr, esp
	Buffer += (BYTE)0x25;
	Buffer += (DWORD)&m_pOrgStack;

	Buffer += (BYTE)0x83; //add esp, 4
	Buffer += (BYTE)0xEC;
	Buffer += (BYTE)0x04;

	Buffer += (BYTE)0xC7; //mov dword ptr [esp],
	Buffer += (BYTE)0x04;
	Buffer += (BYTE)0x24;
	int iOrgReturnAddressIndex = Buffer + (DWORD)0;

	if(!m_bNoRegs)
	{
		Buffer += (BYTE)0x80; //cmp byte ptr
		Buffer += (BYTE)0x3D;
		Buffer += (DWORD)&m_bDontReplaceOrgClassInstance;
		Buffer += (BYTE)0x00;

		Buffer += (BYTE)0x0F; //cmove ecx, dword ptr
		Buffer += (BYTE)0x44;
		Buffer += (BYTE)0x0D;
		Buffer += (DWORD)&m_pClassInstance;

		Buffer += (BYTE)0xC6; //mov byte ptr
		Buffer += (BYTE)0x05;
		Buffer += (DWORD)&m_bDontReplaceOrgClassInstance;
		Buffer += (BYTE)0x00;
	}

	int iOverwrittenOpsIndex = Buffer.Peek();

	int iOverwrittenOps = 0;
	int iOverwrittenBytes = 0;

	CDetourDis Dis(NULL, NULL);

	BYTE* pbSrc = m_pAddr;

	BYTE* pbLastOp = pbSrc;

	if(m_iBytesToOverwrite > 0)
	{
		iOverwrittenBytes = m_iBytesToOverwrite;

		pbSrc += iOverwrittenBytes;

		Buffer.Grow(iOverwrittenBytes);

	} else {
		while(iOverwrittenBytes < iRawDetourSize)
		{
			pbLastOp = pbSrc;

			if(*pbSrc == OP_BRK)
				break;

			BYTE* pbNew = Dis.CopyInstruction(NULL, pbSrc);

			iOverwrittenOps++;

			int iDelta = (int)(pbNew - pbSrc);

			if((pbNew == NULL) || (iDelta == 0))
				return false;

			iOverwrittenBytes += iDelta;

			pbSrc += iDelta;

			Buffer.Grow(iDelta);

			pbSrc = pbNew;
		}
	}

	m_iDetourSize = iOverwrittenBytes;

	m_pOrgAfterDetour = pbSrc;

	//-----

	if(!(*pbLastOp == OP_BRK || *pbLastOp == OP_NOP))	//align [end of function]
	{
		Buffer += (BYTE)0xFF; //jmp dword ptr
		Buffer += (BYTE)0x25;
		Buffer += (DWORD)&m_pOrgAfterDetour;
	}

	int iOrgReturnAddressOffset = Buffer.Peek();

	Buffer += (BYTE)0x8B; //mov esp, dword ptr
	Buffer += (BYTE)0x25;
	Buffer += (DWORD)&m_pOrgStack;

	Buffer += (BYTE)0xFF; //jmp dword ptr
	Buffer += (BYTE)0x25;
	Buffer += (DWORD)&m_pOrgReturnAddress;

	m_pOrgGateBuf = Buffer.Copy();

	*(DWORD*)&m_pOrgGateBuf[iOrgReturnAddressIndex] = (DWORD)&m_pOrgGateBuf[iOrgReturnAddressOffset];

	BYTE* pbDst = &m_pOrgGateBuf[iOverwrittenOpsIndex];
	pbSrc = (BYTE*)m_pAddr;

	if(m_iBytesToOverwrite > 0)
	{
		memcpy(pbDst, pbSrc, iOverwrittenBytes);

	} else {
		for(int iCurOp = 0; iCurOp < iOverwrittenOps; iCurOp++)
		{
			BYTE* pbNew = Dis.CopyInstruction(pbDst, pbSrc);

			pbDst += (pbNew - pbSrc);

			pbSrc = pbNew;
		}

	}

	if(!GenerateDetour(Buffer, m_pAddr, m_pGateBuf, m_iDetourSize))
		return false;

	m_pDetourBuf = Buffer.Copy();

	Buffer.Clear();

	Org = (CDetourOrg_Func)m_pOrgGateBuf;

	m_pOrgBuf = (BYTE*)calloc(m_iDetourSize, sizeof(BYTE));

	memcpy(m_pOrgBuf, m_pAddr, m_iDetourSize);

	m_bReady = true;

	return true;
}

int CSimpleDetour::g_iApplied = 0;
int CSimpleDetour::g_iRemoved = 0;

int CSimpleDetour::Applied()
{
	return g_iApplied;
}

int CSimpleDetour::Removed()
{
	return g_iRemoved;
}

CSimpleDetour::CSimpleDetour()
{
	m_pDetourBuf = NULL;
	m_pOrgBuf = NULL;
	m_pGateBuf = NULL;

	Deconstruct();
};

CSimpleDetour::~CSimpleDetour()
{
	Deconstruct();
};

void CSimpleDetour::Deconstruct()
{
	m_bReady = false;

	if(m_bApplied)
		Remove();

	FreeBuffers();

	m_iBytesToOverwrite = 0;

	m_pAddr = NULL;
	m_pFuncToCall = NULL;

	m_iDetourSize = 0;

	m_pRetAddress = 0;
}

void CSimpleDetour::FreeBuffers()
{
	m_bReady = false;

	if(m_pDetourBuf)
		free(m_pDetourBuf);

	if(m_pOrgBuf)
		free(m_pOrgBuf);

	if(m_pGateBuf)
		free(m_pGateBuf);

	m_pDetourBuf = NULL;
	m_pOrgBuf = NULL;
	m_pGateBuf = NULL;
}

bool CSimpleDetour::Detour(BYTE* pAddr, BYTE* pFuncToCall, bool bExecuteOverwrittenOps, int iBytesToOverwrite)
{
	Deconstruct();

	if(!pAddr || !pFuncToCall)
		return false;

	m_pAddr = pAddr;
	m_pFuncToCall = pFuncToCall;

	m_bExecuteOverwrittenOps = bExecuteOverwrittenOps;
	m_iBytesToOverwrite = iBytesToOverwrite;

	return Generate();
}


bool CSimpleDetour::WriteToAddr(BYTE* pBuffer, int iSize)
{
	if(!m_bReady)
		return false;

	Patch(m_pAddr, pBuffer, iSize);

	return true;
}

bool CSimpleDetour::Apply()
{
	if(!m_bReady || m_bApplied)
		return false;

	if(!WriteToAddr(m_pDetourBuf, m_iDetourSize))
		return false;

	m_bApplied = true;

	g_iApplied++;

	return true;
}

bool CSimpleDetour::Remove()
{
	if(!m_bApplied)
		return false;

	if(!WriteToAddr(m_pOrgBuf, m_iDetourSize))
		return false;

	m_bApplied = false;

	g_iRemoved++;

	return true;
}

bool CSimpleDetour::IsReady()
{
	return m_bReady;
}

bool CSimpleDetour::IsApplied()
{
	return m_bApplied;
}

BYTE* CSimpleDetour::GetAddr()
{
	return m_pAddr;
}

BYTE* CSimpleDetour::GetFuncToCall()
{
	return m_pFuncToCall;
}

void CSimpleDetour::SetFuncToCall(BYTE* pFuncToCall)
{
	m_pFuncToCall = pFuncToCall;
}

BYTE* CSimpleDetour::GetRetAddress()
{
	return m_pRetAddress;
}

bool CSimpleDetour::Generate()
{
	FreeBuffers();

	LVCByteArray Buffer;

	Buffer.Clear();

	Buffer += (BYTE)0xE8; //call
	int iDetourOffsetIndex = Buffer + (DWORD)0;

	BYTE* pRawDetourBuf = Buffer.Copy();

	int iRawDetourSize = Buffer.Peek();

	if(m_iBytesToOverwrite > 0)
		if(m_iBytesToOverwrite < iRawDetourSize)
			return false;

	Buffer.Clear();

	Buffer += (BYTE)0x8F; //pop dword ptr
	Buffer += (BYTE)0x05;
	Buffer += (DWORD)&m_pRetAddress;

	Buffer += (BYTE)0xFF; //call dword ptr
	Buffer += (BYTE)0x15;
	Buffer += (DWORD)&m_pFuncToCall;

	int iOverwrittenOpsIndex = Buffer.Peek();

	int iOverwrittenOps = 0;
	int iOverwrittenBytes = 0;

	CDetourDis Dis(NULL, NULL);

	BYTE* pbSrc = m_pAddr;

	if(m_iBytesToOverwrite > 0)
	{
		iOverwrittenBytes = m_iBytesToOverwrite;

		pbSrc += iOverwrittenBytes;

		Buffer.Grow(iOverwrittenBytes);

	} else {
		while(iOverwrittenBytes < iRawDetourSize)
		{
			if(*pbSrc == OP_BRK)
				break;

			BYTE* pbNew = Dis.CopyInstruction(NULL, pbSrc);

			iOverwrittenOps++;

			int iDelta = (int)(pbNew - pbSrc);

			if((pbNew == NULL) || (iDelta == 0))
				return false;

			iOverwrittenBytes += iDelta;

			pbSrc += iDelta;

			Buffer.Grow(iDelta);

			pbSrc = pbNew;
		}
	}

	m_iDetourSize = iOverwrittenBytes;

	Buffer += (BYTE)0xFF; //jmp dword ptr
	Buffer += (BYTE)0x25;
	Buffer += (DWORD)&m_pRetAddress;

	m_pGateBuf = Buffer.Copy();

	BYTE* pbDst = &m_pGateBuf[iOverwrittenOpsIndex];
	pbSrc = m_pAddr;

	if(m_iBytesToOverwrite > 0)
	{
		memcpy(pbDst, pbSrc, iOverwrittenBytes);

	} else {
		for(int iCurOp = 0; iCurOp < iOverwrittenOps; iCurOp++)
		{
			BYTE* pbNew = Dis.CopyInstruction(pbDst, pbSrc);

			pbDst += (pbNew - pbSrc);

			pbSrc = pbNew;
		}

	}

	*(int*)&pRawDetourBuf[iDetourOffsetIndex] = m_pGateBuf - m_pAddr - 5;

	m_pDetourBuf = (BYTE*)calloc(m_iDetourSize, sizeof(BYTE));

	memset(m_pDetourBuf, OP_NOP, m_iDetourSize);

	memcpy_s(m_pDetourBuf, m_iDetourSize, pRawDetourBuf, iRawDetourSize);

	free(pRawDetourBuf);

	pRawDetourBuf = NULL;

	m_pOrgBuf = (BYTE*)calloc(m_iDetourSize, sizeof(BYTE));

	memcpy_s(m_pOrgBuf, m_iDetourSize, m_pAddr, m_iDetourSize);

	m_bReady = true;

	return true;
}

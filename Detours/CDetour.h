#pragma once
#include "LVCByteArray.h"

typedef void* (__cdecl* CDetourOrg_Func)( ... );

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Universal Detour Class
/// 
/// @author	LanceVorgin
////////////////////////////////////////////////////////////////////////////////////////////////////
class CDetour
{
public:
	static int Applied();
	static int Removed();
	static void InitRand();

private:
	static int g_iApplied;
	static int g_iRemoved;

	static void* __cdecl NullOrgFunc( ... );
	static CDetourOrg_Func NullOrg;

public:
	CDetour();
	~CDetour();

	bool Detour(BYTE* pAddr, BYTE* pFuncToCall, bool bAutoReturn = false, bool bNoRegs = false, bool bPolymorphic = false, int iArgsOverride = -1, int iBytesToOverwrite = -1);
	bool Detour(LPCSTR lpLibraryName, LPCSTR lpProcName, BYTE* pFuncToCall, bool bAutoReturn = false, bool bNoRegs = false, bool bPolymorphic = false, int iArgsOverride = -1, int iBytesToOverwrite = -1);

	bool Apply();
	bool Remove();

	bool IsReady();
	bool IsApplied();

	BYTE* GetAddr();

	BYTE* GetFuncToCall();
	void SetFuncToCall(BYTE* pFuncToCall);

	BYTE* GetRetAddress();

	BYTE* GetGateRetAddress();
	void SetGateRetAddress(BYTE* pGateRetAddress);
	
	BYTE* GetThisPtr();
	void SetThisPtr(BYTE* pThisPtr);

	void NoSetThisPtr(bool bNoSetThisPtr);

	void Ret(bool bReturnToOriginal);

	CDetourOrg_Func Org;

private:
	void Deconstruct();
	void FreeBuffers();

	bool Generate();
	
	int GetDetourSize();
	bool GenerateDetour(LVCByteArray& Buffer, BYTE* pBase, BYTE* pTarget, int iFinalSize = -1);

	bool WriteToAddr(BYTE* pBuffer, int iSize);

	bool m_bReady;
	bool m_bApplied;

	bool m_bAutoReturn;
	bool m_bNoRegs;
	bool m_bPolymorphic;
	int m_iArgsOverride;
	int m_iBytesToOverwrite;

	BYTE* m_pAddr;
	BYTE* m_pFuncToCall;

	BYTE m_ubDetourType;
	int m_iDetourSize;
	BYTE* m_pOrgAfterDetour;

	BYTE* m_pDetourBuf;
	BYTE* m_pOrgBuf;
	BYTE* m_pGateBuf;
	BYTE* m_pOrgGateBuf;

	bool m_bDontReplaceOrgClassInstance;

	BYTE* m_pGateStack;
	BYTE* m_pGateRealRet;

	bool m_bReturnToOriginal;
	BYTE* m_pAutoReturnEndStack;

	BYTE* m_pRetAddress;
	BYTE* m_pClassInstance;
	BYTE* m_pOrgReturnAddress;
	BYTE* m_pOrgStack;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief	Allows for construction of simple detours.
/// 
/// @author	LanceVorgin
/// @date	7/20/2009
////////////////////////////////////////////////////////////////////////////////////////////////////
class CSimpleDetour
{
public:
	static int Applied();
	static int Removed();

private:
	static int g_iApplied;
	static int g_iRemoved;

public:
	CSimpleDetour();
	~CSimpleDetour();

	bool Detour(BYTE* pAddr, BYTE* pFuncToCall, bool bExecuteOverwrittenOps = true, int iBytesToOverwrite = 0);

	bool Apply();
	bool Remove();

	bool IsReady();
	bool IsApplied();

	BYTE* GetAddr();

	BYTE* GetFuncToCall();
	void SetFuncToCall(BYTE* pFuncToCall);

	BYTE* GetRetAddress();

	void Ret(bool bReturnToOriginal);

private:
	void Deconstruct();
	void FreeBuffers();

	bool Generate();

	bool WriteToAddr(BYTE* pBuffer, int iSize);

	bool m_bReady;
	bool m_bApplied;

	bool m_bExecuteOverwrittenOps;
	int m_iBytesToOverwrite;

	BYTE* m_pAddr;
	BYTE* m_pFuncToCall;

	int m_iDetourSize;
	BYTE* m_pOrgAfterDetour;

	BYTE* m_pDetourBuf;
	BYTE* m_pOrgBuf;
	BYTE* m_pGateBuf;

	BYTE* m_pRetAddress;
};

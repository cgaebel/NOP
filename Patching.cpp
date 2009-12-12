#include "Patching.h"
#include "defs.h"

namespace Patching
{
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	The mid-level function that performs an edit on the code segment.
	///
	///			The function was designed to be safer than purely calling memcpy. It ensures that the new
	///			code that got patched in gets called, instead of simply the cached copy of it.
	/// 
	/// @author	Clark Gaebel
	/// @date	7/2/2009
	/// 
	/// @param	_Dst		If non-null, destination for the patch.
	/// @param	_Src		If non-null, source for the patch.
	/// @param	patchSize	Size of the patch. 
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void __fastcall Patch(void* _Dst, const void* _Src, size_t patchSize)
	{
		static const HANDLE currentProcess = GetCurrentProcess();

		DWORD ulOldProtect[2];
		VirtualProtect(_Dst, patchSize, PAGE_EXECUTE_READWRITE, &(ulOldProtect[0]));
		FlushInstructionCache(currentProcess, _Dst, patchSize);
		memcpy_s(_Dst, patchSize, _Src, patchSize);
		VirtualProtect(_Dst, patchSize, ulOldProtect[0], &(ulOldProtect[1]));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Patches an unconditional jump (JMP) from one location in memory that's more than 0x80
	///			away.
	/// 
	/// @author	Clark Gaebel
	/// @date	8/10/2009
	/// 
	/// @param	_Src	The source of the jump. Also where the patch will be made.
	/// @param	delta	Distance between the source and the destination of the jump.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void __fastcall PatchLongJump(DWORD _Src, signed long delta)
	{
		BYTE jumpBuffer[1 + sizeof(delta)];

		jumpBuffer[0] = 0xE9;

		// The payload of the instruction is the distance to the address to jump to.
		*(DWORD*)(jumpBuffer + 1) = (delta < 0) ? (DWORD)(delta + sizeof(jumpBuffer)) : (DWORD)(delta - sizeof(jumpBuffer));

		// Now patch the jump into its source.
		Patch((void*)_Src, jumpBuffer, sizeof(jumpBuffer));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Patches a short jump that jumps from _Src and the amount is delta bytes. The memory
	///			must be less than 0x80 away.
	/// 
	/// @author	Clark Gaebel
	/// @date	8/12/2009
	/// 
	/// @param	_Src	Source of the jump. Also where the patch will be made.
	/// @param	delta	Distance between the source and the destination of the jump.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void __fastcall PatchShortJump(DWORD _Src, signed char delta)
	{
		BYTE jumpBuffer[1 + sizeof(delta)];

		jumpBuffer[0] = 0xEB;

		*(BYTE*)(jumpBuffer + 1) = (delta < 0) ? ((BYTE)(delta + sizeof(jumpBuffer))) : ((BYTE)(delta - sizeof(jumpBuffer)));

		Patch((void*)_Src, jumpBuffer, sizeof(jumpBuffer));
	}

	#pragma warning(push)
	#pragma warning(disable:4244)
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief	Patches an unconditional jump at _Src. The location of the jump will be _Dst.
	/// 
	/// @author	Clark Gaebel
	/// @date	8/13/2009
	/// 
	/// @param	_Src	Source of the jump. Also where the patch will be applied.
	/// @param	_Dst	Destination for the jump.
	////////////////////////////////////////////////////////////////////////////////////////////////////
	void __fastcall PatchUnconditionalJump(DWORD _Src, DWORD _Dst)
	{
		DWORD jmpPayload = GetDistanceToAddress(_Src, _Dst, 5);

		if(jmpPayload < 256)
		{
			BYTE jmpBuffer[1 + sizeof(BYTE)];
			jmpBuffer[0] = 0xEB;
			*(BYTE*)(jmpBuffer + 1) = (BYTE)jmpPayload;
			Patch((void*)_Src, jmpBuffer, sizeof(jmpBuffer));
		}
		else
		{
			BYTE jmpBuffer[1 + sizeof(DWORD)];
			jmpBuffer[0] = 0xE9;
			*(DWORD*)(jmpBuffer + 1) = (DWORD)jmpPayload;
			Patch((void*)_Src, jmpBuffer, sizeof(jmpBuffer));
		}
	}
}

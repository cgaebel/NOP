#pragma once

namespace Patching
{
	void __fastcall Patch(void* _Dst, const void* _Src, size_t patchSize);

	inline DWORD GetDistanceToAddress(DWORD source, DWORD destination, size_t bufferSize = 5)
	{
		return destination - source - bufferSize;	// Integer overflow is not only permitted here, but desired behaviour.
	}

	void __fastcall PatchLongJump(DWORD _Src, signed long delta);
	void __fastcall PatchShortJump(DWORD _Src, signed char delta);

	void __fastcall PatchUnconditionalJump(DWORD _Src, DWORD _Dst);
}

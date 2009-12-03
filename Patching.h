#pragma once

namespace Patching
{
	void __fastcall Patch(void* _Dst, const void* _Src, size_t patchSize);

	DWORD GetDistanceToAddress(DWORD source, DWORD destination, size_t bufferSize = 5);

	void __fastcall PatchLongJump(DWORD _Src, signed long delta);
	void __fastcall PatchShortJump(DWORD _Src, signed char delta);

	void __fastcall PatchUnconditionalJump(DWORD _Src, DWORD _Dst);
}

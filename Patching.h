#pragma once

namespace Patching
{
	void __fastcall Patch(void* _Dst, const void* _Src, size_t patchSize);

	void __fastcall PatchLongJump(DWORD _Src, signed long delta);
	void __fastcall PatchShortJump(DWORD _Src, signed char delta);

	void __fastcall PatchUnconditionalJump(DWORD _Src, DWORD _Dst);
}

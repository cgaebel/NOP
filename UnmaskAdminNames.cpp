#include "Core.h"
#include "Patching.h"

INITIALIZER(UnmaskAdminNames, "Unmasking admin names...")
{
	#define LEA_THEN_NOP(address, secondByte)		(Patching::Patch((void*)(address), "\x8D" secondByte "\xD0\x03\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90\x90", 15))
#define MINI_LEA_THEN_NOP(address, secondByte)	(Patching::Patch((void*)(address), "\x8D" secondByte "\xD0\x03\x00\x00\x90\x90\x90", 9))
#define SMALL_NOP_SLIDE(address)				(Patching::Patch((void*)(address), "\x90\x90\x90\x90\x90\x90", 6))
#define DOUBLE_NOP_SLIDE(address)				for(;;) { SMALL_NOP_SLIDE((void*)(address)); SMALL_NOP_SLIDE((void*)(address+6)); break; }

	// Unmask the -- that replaces the level. Unfortunately, doesn't work in the tab menu, and possibly duels.
	Patching::Patch((void*)0x00405B57, "\x8B\x86\x64\x04\x00\x00\x8B\x50\x0C\x8B\x48\x54\x8B\x04\x8A", 15);
	Patching::Patch((void*)0x00422142, "\x8D\x44\x24\x60\xC6\x44\xE4\x13\x01\xEB\x15\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 25);
	LEA_THEN_NOP(0x0040A483, "\xAF");
	LEA_THEN_NOP(0x0040A4DF, "\xAF");
	LEA_THEN_NOP(0x0040A516, "\xAF");
	Patching::Patch((void*)0x0040A37A, "\xE9\xD5\x01\x00\x00\x8D\xAF\xD0\x03\x00\x00\xEB\x07\x90\x90\x90\x90\x90\x90\x90", 20);

	// Unmask the Administrator name.
	LEA_THEN_NOP(0x00402CA4, "\x8F");
	LEA_THEN_NOP(0x00403128, "\x85");
	LEA_THEN_NOP(0x0040314C, "\x85");
	LEA_THEN_NOP(0x00403484, "\x87");
	SMALL_NOP_SLIDE(0x004034B8);
	MINI_LEA_THEN_NOP(0x004034C4, "\x8F");
	LEA_THEN_NOP(0x0040377C, "\x86");
	SMALL_NOP_SLIDE(0x004037B0);
	MINI_LEA_THEN_NOP(0x004037BC, "\x8E");
	LEA_THEN_NOP(0x00405B57, "\x86");
	LEA_THEN_NOP(0x0040A483, "\xAF");
	LEA_THEN_NOP(0x0040A4DF, "\xAF");
	LEA_THEN_NOP(0x0040A516, "\xAF");
	LEA_THEN_NOP(0x004102D3, "\x86");
	LEA_THEN_NOP(0x00410307, "\x8E");
	LEA_THEN_NOP(0x00410B56, "\xB9");
	LEA_THEN_NOP(0x00410C9E, "\x8F");
	LEA_THEN_NOP(0x00410D41, "\x8F");
	LEA_THEN_NOP(0x0041124E, "\xB9");
	Patching::Patch((void*)0x004236DE, "\x8D\x0E\x90\x90\x90\x90\x90", 7);
	Patching::Patch((void*)0x004383F8, "\x74", 1);
	Patching::Patch((void*)0x0043CBA2, "\x00", 1);
	Patching::Patch((void*)0x0045B81A, "\x75", 1);
	Patching::Patch((void*)0x00508EB2, "\x74", 1);	// TCP Socket disconnected.
	Patching::Patch((void*)0x00519F1C, "\x75", 1);
	Patching::Patch((void*)0x0051AD3A, "\x74", 1);
	Patching::Patch((void*)0x004A6DB9, "\x8D\x87\xD0\x03\x00\x00\xEB\x11\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 20);
	Patching::Patch((void*)0x004A6DF1, "\x8D\x86\xD0\x03\x00\x00\xEB\x11\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 20);
	Patching::Patch((void*)0x004A74BC, "\x8D\x93\xD0\x03\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 18);
	Patching::Patch((void*)0x004A75CB, "\x8D\x92\xD0\x03\x00\x00", 6);
	DOUBLE_NOP_SLIDE(0x004A75E6);
	Patching::Patch((void*)0x004A9117, "\x8D\x8E\x48\x03\x00\x00\x90", 7);
	Patching::Patch((void*)0x004A9298, "\x8D\x97\x48\x03\x00\x00\x90", 7);
	Patching::Patch((void*)0x004C318D, "\x57\x8D\x8C\x24\x20\x01\x00\x00\x90\x90\x90\x90", 12);
	Patching::Patch((void*)0x004C4EB7, "\x8D\x57\x0C\x90", 4);
	Patching::Patch((void*)0x004C609A, "\xEB", 1);
	SMALL_NOP_SLIDE(0x004C7063);
	Patching::Patch((void*)0x004C707E, "\x8D\x86\xD0\x03\x00\x00\x90\x90\x90\x90\x90\x90", 12);
	SMALL_NOP_SLIDE(0x004C7123);
	Patching::Patch((void*)0x004C713E, "\x8D\x86\xD0\x03\x00\x00\x90\x90\x90\x90\x90\x90", 12);
	Patching::Patch((void*)0x004078CB, "\x8D\x81\xD0\x03\x00\x00", 6);
	Patching::Patch((void*)0x004078D1, "\xEB\x01", 2);
	Patching::Patch((void*)0x004078D3, "\x90", 1);

	// Make the UGradeID of 254 (Developer) A nice shade of blue:
	Patching::Patch((void*)0x0049EF18, "\xC6\x44\xE4\x08\x10\xC6\x44\xE4\x09\x82\x88\x44\xE4\x0A\x88\x44\xE4\x0B\x8B\x44\xE4\x0C", 22);

	// Make the admin wall say the "shouter"s name on announce. Consists of a codecave and a jump to said codecave.
	Patching::Patch((void*)0x0042CC87, "\xE9\x54\x84\x1B\x00", 5);

	Patching::Patch((void*)0x005E50E0,	"\x50\x53\x51\x52\x57\xBF\x3C\xC7\x66\x00\xBA\x00\x00\x6F\x00\x2B\xC9\x8A\x04"
									"\x39\x3C\x00\x74\x06\x88\x04\x11\x41\xEB\xF3\xB0\x20\x88\x04\x11\x41\xB0\x3A"
									"\x88\x04\x11\x41\xB0\x20\x88\x04\x11\x41\x03\xD1\x2B\xC9\x8B\xFC\x83\xC7\x20"
									"\x8A\x04\x39\x3C\x00\x74\x06\x88\x04\x11\x41\xEB\xF3\x88\x04\x11\x66\xC7\x05"
									"\xFE\xFF\x6E\x00\x5E\x32\x5F\x5A\x59\x5B\x58\x68\xFE\xFF\x6E\x00\xE9\x4B\x7B"
									"\xE4\xFF", 97
				);

#undef DOUBLE_NOP_SLIDE
#undef SMALL_NOP_SLIDE
#undef MINI_LEA_THEN_NOP
#undef LEA_THEN_NOP
}

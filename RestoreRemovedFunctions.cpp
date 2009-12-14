#include "Core.h"
#include "Patching.h"

// Decryption routines at: 0x00538680, 0x00538960, and 0x00538D12
static void RestoreMRSDecryptionRoutines()
{
	Patching::Patch((void*)0x00538680,
		"\x8B\x4C\x24\x04\x85\xC9\x74\x1E\x56\x8B\x74\x24\x0C\x85\xF6\x7E"
		"\x14\x8A\x01\x90\x90\x2C\x4F\x90\x90\x90\x90\x90\x90\x90\x90\x88"
		"\x01\x41\x4E\x75\xEC\x5E\xC3"
	, 2 * 16 + 7);

	Patching::Patch((void*)0x00538960,
		"\x83\xEC\x58\x53\x8B\x5C\x24\x64\x85\xDB\x56\x8B\xF1\x0F\x84\xBA"
		"\x01\x00\x00\x8B\x44\x24\x64\x85\xC0\x0F\x8C\xAE\x01\x00\x00\x3B"
		"\x46\x10\x0F\x8D\xA5\x01\x00\x00\x8B\x4E\x0C\x8B\x14\x81\x8B\x42"
		"\x2A\x8B\x4E\x04\x57\x6A\x00\x50\x51\xE8\x49\xAD\x03\x00\x8B\x56"
		"\x04\x52\x6A\x01\x8D\x44\x24\x20\x6A\x1E\x50\xE8\xC4\x9B\x03\x00"
		"\x8B\x46\x14\x83\xC4\x1C\x83\xF8\x02\x7C\x1D\x8D\x4C\x24\x0C\xBF"
		"\x1E\x00\x00\x00\x8A\x01\x90\x90\x2C\x4F\x90\x90\x90\x90\x90\x90"
		"\x90\x90\x88\x01\x41\x4F\x75\xEC\x8B\x44\x24\x0C\x3D\x50\x4B\x03"
		"\x04\x74\x12\x3D\x00\x00\x84\x85\x74\x0B\x5F\x5E\x32\xC0\x5B\x83"
		"\xC4\x58\xC2\x0C\x00\x0F\xB7\x44\x24\x28\x0F\xB7\x4C\x24\x26\x8B"
		"\x56\x04\x6A\x01\x03\xC1\x50\x52\xE8\xDA\xAC\x03\x00\x66\x8B\x44"
		"\x24\x20\x83\xC4\x0C\x66\x85\xC0\x75\x1F\x8B\x46\x04\x8B\x4C\x24"
		"\x1E\x50\x6A\x01\x51\x53\xE8\x49\x9B\x03\x00\x83\xC4\x10\x5F\x5E"
		"\xB0\x01\x5B\x83\xC4\x58\xC2\x0C\x00\x66\x3D\x08\x00\x75\xAB\x8B"
		"\x54\x24\x1E\x52\xE8\x91\x65\x09\x00\x8B\xD8\x83\xC4\x04\x85\xDB"
		"\x74\x98\x8B\x4C\x24\x1E\x8B\xD1\xC1\xE9\x02\x33\xC0\x8B\xFB\xF3"
		"\xAB\x8B\xCA\x83\xE1\x03\xF3\xAA\x8B\x46\x04\x50\xE8\xA6\xAB\x03"
		"\x00\x8B\x4E\x04\x8B\x54\x24\x22\x51\x6A\x01\x52\x53\xE8\xF2\x9A"
		"\x03\x00\x8B\x44\x24\x32\x8B\x8C\x24\x80\x00\x00\x00\x89\x44\x24"
		"\x44\x8B\x84\x24\x84\x00\x00\x00\x89\x4C\x24\x4C\x8B\x4C\x24\x36"
		"\x83\xC4\x14\x3B\xC1\xC7\x44\x24\x4C\x00\x00\x00\x00\xC7\x44\x24"
		"\x50\x00\x00\x00\x00\xC7\x44\x24\x54\x00\x00\x00\x00\x89\x5C\x24"
		"\x2C\x89\x44\x24\x3C\x72\x04\x89\x4C\x24\x3C\x6A\x38\x68\xE4\xA5"
		"\x5E\x00\x8D\x54\x24\x34\x6A\xF1\x52\xE8\xF2\x82\xF9\xFF\x8B\xF0"
		"\x83\xC4\x10\x85\xF6\x75\x2F\x8D\x44\x24\x2C\x6A\x04\x50\xE8\xED"
		"\x84\xF9\xFF\x8D\x4C\x24\x34\x51\x8B\xF0\xE8\x41\x99\xF9\xFF\x83"
		"\xC4\x0C\x83\xFE\x01\x75\x02\x33\xF6\x8D\x54\x24\x2C\x52\xE8\x2D"
		"\x99\xF9\xFF\x83\xC4\x04\x53\xE8\xC3\x64\x09\x00\x83\xC4\x04\x5F"
		"\x85\xF6\x5E\x0F\x94\xC0\x5B\x83\xC4\x58\xC2\x0C\x00\x5E\x32\xC0"
		"\x5B\x83\xC4\x58\xC2\x0C\x00"
	, 29 * 16 + 7);

	Patching::Patch((void*)0x00538C20,
		"\x83\xEC\x18\x53\x8B\x5C\x24\x20\x56\x57\x33\xFF\x3B\xDF\x8B\xF1"
		"\x74\x7E\x8B\x44\x24\x2C\x57\x57\x53\x89\x46\x18\xE8\xA6\xAA\x03"
		"\x00\x53\x6A\x01\x8D\x4C\x24\x3C\x6A\x04\x51\x89\x7C\x24\x44\xE8"
		"\x20\x99\x03\x00\x8B\x44\x24\x44\x83\xC4\x1C\x3D\x50\x4B\x03\x04"
		"\x75\x14\xF6\x46\x18\x01\x89\x7E\x14\x75\x50\x5F\x5E\x32\xC0\x5B"
		"\x83\xC4\x18\xC2\x08\x00\x53\xE8\xF4\xFE\xFF\xFF\x83\xC4\x04\x84"
		"\xC0\x74\x1C\x8B\x56\x18\xD1\xEA\xF6\xC2\x01\xC7\x46\x14\x01\x00"
		"\x00\x00\x75\x27\x5F\x5E\x32\xC0\x5B\x83\xC4\x18\xC2\x08\x00\x8B"
		"\x46\x18\xC1\xE8\x02\xA8\x01\xC7\x46\x14\x02\x00\x00\x00\x75\x0B"
		"\x5F\x5E\x32\xC0\x5B\x83\xC4\x18\xC2\x08\x00\x55\x6A\x02\x6A\xEA"
		"\x53\xE8\x21\xAA\x03\x00\x53\xE8\x4B\xA9\x03\x00\x33\xC9\x89\x4C"
		"\x24\x20\x89\x4C\x24\x24\x53\x89\x4C\x24\x2C\x6A\x01\x89\x4C\x24"
		"\x34\x8D\x54\x24\x28\x89\x4C\x24\x38\x6A\x16\x52\x8B\xF8\x66\x89"
		"\x4C\x24\x44\xE8\x7C\x98\x03\x00\x8B\x46\x14\x83\xC4\x20\x83\xF8"
		"\x02\x7C\x21\x8D\x4C\x24\x10\xBD\x16\x00\x00\x00\x8D\x64\x24\x00"
		"\x8A\x01\x90\x90\x2C\x4F\x90\x90\x90\x90\x90\x90\x90\x90\x88\x01"
		"\x41\x4D\x75\xEC\x8B\x44\x24\x10\x3D\x08\x02\x03\x05\x74\x12\x3D"
		"\x07\x02\x03\x05\x74\x0B\x3D\x50\x4B\x05\x06\x0F\x85\x0F\x01\x00"
		"\x00\x8B\x54\x24\x1C\x6A\x00\x2B\xFA\x57\x53\xE8\x97\xA9\x03\x00"
		"\x0F\xB7\x44\x24\x24\x8B\x4C\x24\x28\x8D\x14\x81\x52\xE8\x78\x62"
		"\x09\x00\x8B\x4C\x24\x2C\x8B\xF8\x0F\xB7\x44\x24\x28\x8D\x0C\x81"
		"\x8B\xD1\xC1\xE9\x02\x89\x7E\x08\x33\xC0\xF3\xAB\x8B\xCA\x83\xE1"
		"\x03\x53\xF3\xAA\x8B\x44\x24\x30\x8B\x4E\x08\x6A\x01\x50\x51\xE8"
		"\xE0\x97\x03\x00\x8B\x46\x14\x83\xC4\x20\x83\xF8\x02\x7C\x11\x8B"
		"\x54\x24\x1C\x8B\x46\x08\x52\x50\xE8\xD3\xF8\xFF\xFF\x83\xC4\x08"
		"\x8B\x4C\x24\x1C\x8B\x46\x08\x8D\x14\x08\x66\x8B\x4C\x24\x18\x33"
		"\xFF\x66\x85\xC9\x89\x56\x0C\x76\x5F\x8D\xA4\x24\x00\x00\x00\x00"
		"\x8B\x4E\x0C\x89\x04\xB9\x8B\x08\x81\xF9\x50\x4B\x01\x02\x8B\xD0"
		"\x74\x08\x81\xF9\x80\x4B\x02\x05\x75\x53\x33\xC9\x83\xC0\x2E\x66"
		"\x39\x4A\x1C\x76\x13\x80\x3C\x01\x2F\x75\x04\xC6\x04\x01\x5C\x0F"
		"\xB7\x6A\x1C\x41\x3B\xCD\x7C\xED\x0F\xB7\x4A\x20\x0F\xB7\x6A\x1E"
		"\x0F\xB7\x52\x1C\x03\xCD\x03\xD0\x8D\x04\x0A\x66\x8B\x4C\x24\x18"
		"\x0F\xB7\xD1\x47\x3B\xFA\x7C\xA8\x5D\x0F\xB7\xC9\x5F\x89\x5E\x04"
		"\x89\x4E\x10\x5E\xB0\x01\x5B\x83\xC4\x18\xC2\x08\x00\x8B\x46\x08"
		"\x50\xE8\x99\x61\x09\x00\x83\xC4\x04\xC7\x46\x08\x00\x00\x00\x00"
		"\x5D\x5F\x5E\x32\xC0\x5B\x83\xC4\x18\xC2\x08\x00"
	, 35 * 16 + 12);
}

static void RestorePostBasicInfo()
{
	Patching::Patch((void*)0x004A1BF0,
		"\x64\xA1\x00\x00\x00\x00\x6A\xFF\x68\x0B\xA2\x5D\x00\x50\x64\x89"
		"\x25\x00\x00\x00\x00\x83\xEC\x20\x56\x57\x8B\xF1\xFF\x15\x5C\x65"
		"\x5E\x00\x8B\x4E\x50\x8B\xF8\x8A\x41\x31\x84\xC0\x0F\x84\xA4\x01"
		"\x00\x00\x8B\x01\xFF\x50\x3C\x84\xC0\x74\x28\x8B\x46\x30\x8B\x48"
		"\x18\x8B\x50\x0C\x8B\x04\x8A\xD9\x00\x8B\x46\x50\xD8\xA0\xA8\x05"
		"\x00\x00\xD8\x1D\x80\x75\x5E\x00\xDF\xE0\xF6\xC4\x41\x0F\x84\x73"
		"\x01\x00\x00\xE8\x68\xA0\x00\x00\x8B\x56\x38\x8B\xCF\x2B\xCA\x83"
		"\xF9\x64\x0F\x8C\x5E\x01\x00\x00\x8B\x46\x30\x89\x7E\x38\x8B\x50"
		"\x18\x8B\x40\x0C\x8B\x04\x90\x8B\x08\x8B\x76\x50\x89\x4C\x24\x0C"
		"\x8B\x46\x58\x85\xC0\x75\x05\x8D\x46\x4C\xEB\x09\x8B\x50\x20\x8B"
		"\x40\x0C\x8B\x04\x90\xD9\x00\xE8\xFC\xF9\x0C\x00\x66\x89\x44\x24"
		"\x10\x8B\x46\x58\x85\xC0\x75\x05\x8D\x46\x4C\xEB\x09\x8B\x48\x20"
		"\x8B\x50\x0C\x8B\x04\x8A\xD9\x40\x04\xE8\xDA\xF9\x0C\x00\x66\x89"
		"\x44\x24\x12\x8B\x46\x58\x85\xC0\x75\x05\x8D\x46\x4C\xEB\x09\x8B"
		"\x48\x20\x8B\x50\x0C\x8B\x04\x8A\xD9\x40\x08\xE8\xB8\xF9\x0C\x00"
		"\x66\x89\x44\x24\x14\x8B\x46\x68\xD9\x40\x40\xE8\xA8\xF9\x0C\x00"
		"\x66\x89\x44\x24\x16\x8B\x4E\x68\xD9\x41\x44\xE8\x98\xF9\x0C\x00"
		"\x66\x89\x44\x24\x18\x8B\x56\x68\xD9\x42\x48\xE8\x88\xF9\x0C\x00"
		"\x66\x89\x44\x24\x1A\xD9\x86\x00\x05\x00\x00\xD8\x0D\x38\x07\x5F"
		"\x00\xE8\x72\xF9\x0C\x00\x66\x89\x44\x24\x1C\xD9\x86\x04\x05\x00"
		"\x00\xD8\x0D\x38\x07\x5F\x00\xE8\x5C\xF9\x0C\x00\x66\x89\x44\x24"
		"\x1E\xD9\x86\x08\x05\x00\x00\xD8\x0D\x38\x07\x5F\x00\xE8\x46\xF9"
		"\x0C\x00\x66\x89\x44\x24\x20\x8B\x86\x48\x05\x00\x00\x88\x44\x24"
		"\x22\x8B\x86\x4C\x05\x00\x00\x88\x44\x24\x23\x8B\x86\xE0\x02\x00"
		"\x00\x68\x1C\x27\x00\x00\x88\x44\x24\x28\xE8\xE1\x1D\x02\x00\x6A"
		"\x10\x8B\xF0\xE8\x27\xD2\x12\x00\x83\xC4\x08\x89\x44\x24\x08\x85"
		"\xC0\xC7\x44\x24\x30\x00\x00\x00\x00\x74\x10\x6A\x19\x8D\x4C\x24"
		"\x10\x51\x8B\xC8\xE8\x87\x42\x06\x00\xEB\x02\x33\xC0\x50\x8B\xCE"
		"\xC7\x44\x24\x34\xFF\xFF\xFF\xFF\xE8\x43\x5E\x06\x00\x56\xE8\x3D"
		"\x12\x02\x00\x83\xC4\x04\x8B\x4C\x24\x28\x5F\x5E\x64\x89\x0D\x00"
		"\x00\x00\x00\x83\xC4\x2C\xC3"
	, 30 * 16 + 7);
}

static void RestoreZPostConnect()
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
	, 12 * 16 - 2);
}

INITIALIZER(RestoreRemovedFunctions, "Restoring functions...")
{
	RestoreZPostConnect();
	RestoreMRSDecryptionRoutines();
	RestorePostBasicInfo();
}

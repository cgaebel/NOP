// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the NOP_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// NOP_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef NOP_EXPORTS
#define NOP_API __declspec(dllexport)
#else
#define NOP_API __declspec(dllimport)
#endif

extern "C" __declspec(noreturn) __declspec(dllexport) void __cdecl OnFailure(const char* reason);
extern "C" __declspec(noreturn) __declspec(dllexport) void __cdecl OnHackDetected(const char* message);

extern HINSTANCE g_hInstance;
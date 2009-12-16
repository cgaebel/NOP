#pragma once
#include <windows.h>

namespace Utilities
{
	// The ONCE delegate macro.
	#define ONCE_HELPER(instructionsToRunOnce, onceVar)	\
		{												\
			static auto onceVar = true;					\
														\
			if(onceVar)									\
			{											\
				onceVar = false;						\
				instructionsToRunOnce					\
			}											\
		}

	// Use this macro to run a block of code only the first time it is called.
	#define ONCE(instructionsToRunOnce) \
		ONCE_HELPER(instructionsToRunOnce, __onceVar__##__COUNTER__)

	// Asserts that optimize in release mode. DISABLED DUE TO INTEL CPP BEING STUPID.
	#define ASSERTUME	__noop

	std::string ConvertToString(size_t num);

	template <class _Functor>
	int __stdcall CreateThreadProxy(_Functor* functor)
	{
		(*functor)();

		delete functor;
		return 0;
	}

	template <class _Functor>
	void CreateThread(_Functor toRun)
	{
		auto* proxy = new _Functor;
		*proxy = toRun;

		::CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CreateThreadProxy<_Functor>, proxy, NULL, NULL);
	}
}

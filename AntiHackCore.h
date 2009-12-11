#pragma once
#include "ProtectionModules.h"

class AntiHackCore
{
	std::vector<ActiveProtectionFunc*> activeProtectionMechanisms;

private:
	inline AntiHackCore() { }
	static DWORD ActiveProtectionThread(AntiHackCore* simulatedThis);
public:
	static AntiHackCore& Get();

	void AddActiveProtection(ActiveProtectionFunc* functor);
	void AddPassiveProtection(PassiveProtectionFunc* functor);

	// Starts up the active protection thread.
	void BeginActiveProtection();
};
inline AntiHackCore& GetAntiHackCore() { return AntiHackCore::Get(); }

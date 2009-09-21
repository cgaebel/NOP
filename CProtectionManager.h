#pragma once

// Both hack detection function types (active and passive) should return __FUNCTION__ if a hack
// is detected, and NULL otherwise.

// All active protection functions should be of this type (They're run many times,
// not just once. Constantly checking that the hack has not been detected and reacting accordingly)
typedef const char* (ActiveProtectionFunc)();

// All passive protection functions should be of this type (They're run only once, set up their
// trap, then die)
typedef const char* (PassiveProtectionFunc)();

class CProtectionManager
{
	std::vector<ActiveProtectionFunc*> activeProtectionMechanisms;

private:
	static DWORD ActiveProtectionThread(CProtectionManager* simulatedThis);
public:
	static CProtectionManager* Get();

	void AddActiveProtection(ActiveProtectionFunc* functor);
	void AddPassiveProtection(PassiveProtectionFunc* functor);

	// Starts up the active protection thread.
	void BeginActiveProtection();
};

// Protection modules:
ActiveProtectionFunc TrainerDetection;
ActiveProtectionFunc CodeSegmentCheck;
ActiveProtectionFunc DetectDebuggers;
ActiveProtectionFunc APIHookCheck;
ActiveProtectionFunc ModuleCount;

PassiveProtectionFunc ModuleHiding;
PassiveProtectionFunc CheckReturnAddress;
PassiveProtectionFunc FileHash;
PassiveProtectionFunc RestoreZPostConnect;
PassiveProtectionFunc RestorePostBasicInfo;
#pragma once

// Both hack detection function types (active and passive) should return one of these values.
#define NO_HACK_DETECTED	NULL
#define HACK_DETECTED		__FUNCTION__

// All active protection functions should be of this type (They're run many times,
// not just once. Constantly checking that the hack has not been detected and reacting accordingly)
typedef const char* (ActiveProtectionFunc)();

// All passive protection functions should be of this type (They're run only once, set up their
// trap, then die; or, alternatively, can run a protection method once and only once on startup)
typedef const char* (PassiveProtectionFunc)();

// Protection modules:
ActiveProtectionFunc TrainerDetection;
ActiveProtectionFunc CodeSegmentCheck;

PassiveProtectionFunc HideModule;
PassiveProtectionFunc CheckReturnAddress;
PassiveProtectionFunc FileHash;
PassiveProtectionFunc RestoreRemovedFunctions;
PassiveProtectionFunc HideFromPEB;

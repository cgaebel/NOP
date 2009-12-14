#pragma once
#include "InitializationModule.h"
#include "PassiveProtectionModule.h"
#include "ActiveProtectionModule.h"

#include "Ignore.h"

// For fine control over what happens when, you can call each of these functions in turn.
void Initialize();
void RunPassiveProtection();
void BeginActiveProtection();

// To just let the core handle everything, call this.
void StartAntiHack();

#pragma once
#include <list>

class InitializationModule;
class PassiveProtectionModule;
class ActiveProtectionModule;

std::list<InitializationModule*>& GetInitializationList();
std::list<PassiveProtectionModule*>& GetPassiveProtectionList();
std::list<ActiveProtectionModule*>& GetActiveProtectionList();

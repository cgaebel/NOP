#pragma once
#include <memory>
#include <list>

class InitializationModule;
class PassiveProtectionModule;
class ActiveProtectionModule;

std::list<std::tr1::shared_ptr<InitializationModule> >& GetInitializationList();
std::list<std::tr1::shared_ptr<PassiveProtectionModule> >& GetPassiveProtectionList();
std::list<std::tr1::shared_ptr<ActiveProtectionModule> >& GetActiveProtectionList();

std::list<const char*>& GetIgnoreList();

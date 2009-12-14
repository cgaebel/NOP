#include "ListSingletons.h"

std::list<InitializationModule*>& GetInitializationList()
{
	static std::list<InitializationModule*> instance;
	return instance;
}

std::list<PassiveProtectionModule*>& GetPassiveProtectionList()
{
	static std::list<PassiveProtectionModule*> instance;
	return instance;
}

std::list<ActiveProtectionModule*>& GetActiveProtectionList()
{
	static std::list<ActiveProtectionModule*> instance;
	return instance;
}

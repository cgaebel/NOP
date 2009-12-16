#include "ListSingletons.h"

std::list<std::tr1::shared_ptr<InitializationModule> >& GetInitializationList()
{
	static std::list<std::tr1::shared_ptr<InitializationModule> > instance;
	return instance;
}

std::list<std::tr1::shared_ptr<PassiveProtectionModule> >& GetPassiveProtectionList()
{
	static std::list<std::tr1::shared_ptr<PassiveProtectionModule> > instance;
	return instance;
}

std::list<std::tr1::shared_ptr<ActiveProtectionModule> >& GetActiveProtectionList()
{
	static std::list<std::tr1::shared_ptr<ActiveProtectionModule> > instance;
	return instance;
}

std::list<const char*>& GetIgnoreList()
{
	static std::list<const char*> instance;
	return instance;
}

#include "ListSingletons.h"

#define MAKE_LIST(type, name)	\
	std::list<std::tr1::shared_ptr<type> >& name()
	{
		static std::list<std::tr1::shared_ptr<type> > instance;
		return instance;
	}

MAKE_LIST(InitializationModule, GetInitializationList)
MAKE_LIST(PassiveProtectionModule, GetPassiveProtectionList)
MAKE_LIST(ActiveProtectionModule, GetActiveProtectionList)

std::list<const char*>& GetIgnoreList()
{
	static std::list<const char*> instance;
	return instance;
}

#pragma once
#include <list>

template <class ModuleClass>
int ListAdder(std::list<ModuleClass*>& list, ModuleClass* module)
{
	list.push_back(module);
	return 0;
}

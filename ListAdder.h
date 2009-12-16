#pragma once
#include <list>

template <class ModuleClass>
int ListAdder_Basic(std::list<ModuleClass>& list, ModuleClass module)
{
	list.push_back(module);
	return 0;
}

template <class ModuleClass>
int ListAdder_SharedPtr(std::list<std::tr1::shared_ptr<ModuleClass> >& list, std::tr1::shared_ptr<ModuleClass> module)
{
	list.push_back(module);
	return 0;
}

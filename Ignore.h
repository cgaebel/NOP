#pragma once
#include "ListSingletons.h"
#include "ListAdder.h"

#define IGNORE_MODULE(moduleName)	\
	static auto __ignore__##moduleName = ListAdder_Basic(GetIgnoreList(), #moduleName)

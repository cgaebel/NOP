#pragma once
#include "ListSingletons.h"
#include "ListAdder.h"

#define IGNORE_MODULE(moduleName)	\
	static int ignore__##moduleName = ListAdder(GetIgnoreList(), #moduleName)

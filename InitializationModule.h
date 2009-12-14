#pragma once
#include "ListSingletons.h"
#include "ListAdder.h"

class InitializationModule
{
public:
	const char* moduleName;
	const char* logMessage;

	virtual void Run() const = 0;
};

#define INITIALIZER(name, loadingMessage)																		\
	class Initializer##name : public InitializationModule														\
	{																											\
	public:																										\
		Initializer##name()																						\
		{																										\
			moduleName = #name;																					\
			logMessage = loadingMessage;																		\
		}																										\
																												\
		void Run() const;																						\
	};																											\
																												\
	static int adder##name = ListAdder<InitializationModule>(GetInitializationList(), new Initializer##name);	\
																												\
	void Initializer##name::Run() const

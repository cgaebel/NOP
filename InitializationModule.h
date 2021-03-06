#pragma once
#include "Module.h"
#include "ListSingletons.h"
#include "ListAdder.h"

class InitializationModule : public Module
{
public:
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
	static auto adder##name = ListAdder_SharedPtr(GetInitializationList(),										\
		std::tr1::shared_ptr<InitializationModule>(new Initializer##name)										\
	);																											\
																												\
	void Initializer##name::Run() const

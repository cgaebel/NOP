#pragma once
#include "ListSingletons.h"
#include "ListAdder.h"

class PassiveProtectionModule
{
public:
	const char* moduleName;
	const char* logMessage;

	virtual const char* Run() const = 0;
};

#define PASSIVE_PROTECTION(name, loadingMessage)																\
	class PProtection##name : public PassiveProtectionModule													\
	{																											\
	private:																									\
		static const char* HACK_DETECTED;																		\
		static const char* NO_HACK_DETECTED;																	\
	public:																										\
		PProtection##name()																						\
		{																										\
			moduleName = #name;																					\
			logMessage = loadingMessage;																		\
		}																										\
																												\
		const char* Run() const;																				\
	};																											\
																												\
	const char* PProtection##name::HACK_DETECTED = #name;														\
	const char* PProtection##name::NO_HACK_DETECTED = NULL;														\
																												\
	static int adder##name = ListAdder<PassiveProtectionModule>(GetPassiveProtectionList(), new PProtection##name);	\
																												\
	const char* PProtection##name::Run() const

#pragma once
#include "Module.h"
#include "ListSingletons.h"
#include "ListAdder.h"

class PassiveProtectionModule : public Module
{
public:
	const char* logMessage;

	virtual const bool Run() const = 0;
};

#define PASSIVE_PROTECTION(name, loadingMessage)																\
	class PProtection##name : public PassiveProtectionModule													\
	{																											\
	private:																									\
		static const bool HACK_DETECTED;																		\
		static const bool NO_HACK_DETECTED;																		\
	public:																										\
		PProtection##name()																						\
		{																										\
			moduleName = #name;																					\
			logMessage = loadingMessage;																		\
		}																										\
																												\
		const bool Run() const;																					\
	};																											\
																												\
	const bool PProtection##name::HACK_DETECTED = true;															\
	const bool PProtection##name::NO_HACK_DETECTED = false;														\
																												\
	static auto adder##name = ListAdder<PassiveProtectionModule>(GetPassiveProtectionList(), new PProtection##name);	\
																												\
	bool const PProtection##name::Run() const

#pragma once
#include "Module.h"
#include "ListSingletons.h"
#include "ListAdder.h"

class ActiveProtectionModule : public Module
{
public:
	virtual bool Run() const = 0;
};

#define ACTIVE_PROTECTION(name)																					\
	class AProtection##name : public ActiveProtectionModule														\
	{																											\
	private:																									\
		static const bool HACK_DETECTED;																		\
		static const bool NO_HACK_DETECTED;																		\
	public:																										\
		AProtection##name()																						\
		{																										\
			moduleName = #name;																					\
		}																										\
																												\
		bool Run() const;																						\
	};																											\
																												\
	const bool AProtection##name::HACK_DETECTED = true;															\
	const bool AProtection##name::NO_HACK_DETECTED = false;														\
																												\
	static auto adder##name = ListAdder_SharedPtr(GetActiveProtectionList(),									\
		std::tr1::shared_ptr<ActiveProtectionModule>(new AProtection##name)										\
	);																											\
																												\
	bool AProtection##name::Run() const

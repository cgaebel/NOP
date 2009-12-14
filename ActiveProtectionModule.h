#pragma once
#include "ListSingletons.h"
#include "ListAdder.h"

class ActiveProtectionModule
{
public:
	const char* moduleName;
	const char* logMessage;

	virtual const char* Run() const = 0;
};

#define ACTIVE_PROTECTION(name, loadingMessage)																	\
	class AProtection##name : public ActiveProtectionModule														\
	{																											\
	private:																									\
		static const char* HACK_DETECTED;																		\
		static const char* NO_HACK_DETECTED;																	\
	public:																										\
		AProtection##name()																						\
			: moduleName(#name), logMessage(loadingMessage)														\
		{																										\
		}																										\
																												\
		void Run() const;																						\
	};																											\
																												\
	AProtection##name::HACK_DETECTED = #name;																	\
	AProtection##name::NO_HACK_DETECTED = NULL;																	\
																												\
	static int adder##name = ListAdder<ActiveProtectionModule>(GetActiveProtectionList(), new AProtection##name);	\
																												\
	void AProtection##name::Run() const

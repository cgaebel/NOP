#pragma once
#include "CriticalSection.h"

template <typename _T>
class Singleton
{
private:
	CriticalSection critSection;

public:
	static _T& Get();
	void RequestResources();
	void ReleaseResources();
};

template <typename _T>
_T& Singleton<_T>::Get()
{
	static _T instance;
	return instance;
}

template <typename _T>
void Singleton<_T>::RequestResources()
{
	critSection.Enter();
}

template <typename _T>
void Singleton<_T>::ReleaseResources()
{
	critSection.Exit();
}

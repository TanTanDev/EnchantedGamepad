#pragma once
#include <assert.h>

template<class T>
class Singleton
{
public:
	static T& GetInstance()
	{
		static T inst;
		return inst;
	}
};
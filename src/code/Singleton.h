#pragma once

// lazy mans singleton
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
#pragma once
#include <string>
typedef struct lua_State lua_State;
class Script
{
public:
	Script();
	~Script();
	void Load(const char* fileName);
	void Unload();
	void Run(float dt);
	bool Get(const char* name, int& value);

	const std::string& GetFileName();
	const bool HasUpdateFunction();
	lua_State* GetLuaState();
private:
	std::string fileName;
	lua_State* luaState;
	int luaRefUpdate;
	bool scriptLoaded;
};
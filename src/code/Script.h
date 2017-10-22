#pragma once
typedef struct lua_State lua_State;
class Script
{
public:
	Script();
	~Script();
	void Load(const char* fileName);
	void Run(float dt);

	bool Get(const char* name, int& value);
private:
	lua_State* luaState;
	int luaRefUpdate;
	bool scriptLoaded;
};
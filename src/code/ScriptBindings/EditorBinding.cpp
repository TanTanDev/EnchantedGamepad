//#include "VectorBinding.h"
//#include "Vector.h"
#include "EditorBinding.h"
#include "ScriptBinding.h"
//#include <math.h>

extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

namespace ScriptBindings
{
	const char tableName[] = "Bind";

	//static int Binding_x(lua_State* L)
	//{
	//	Vector* Vector = lua_checkvector(L, 1);
	//	if (lua_isnumber(L, 2))
	//	{
	//		Vector->x = static_cast<float>(luaL_checknumber(L, 2));
	//		return 0;
	//	}
	//	else if (lua_isnone(L, 2))
	//	{
	//		lua_pushnumber(L, Vector->x);
	//		return 1;
	//	}
	//	return 1;
	//}
	int SetGlobal(lua_State* L, Binding& binding)
	{
		lua_getglobal(L, binding.GlobalName.c_str());
		if (binding.bindingDataType == Binding::BindingDataType::Float)
			lua_pushnumber(L, binding.bindingData.f);
		else if (binding.bindingDataType == Binding::BindingDataType::Bool)
			lua_pushboolean(L, binding.bindingData.b);
		//		lua_set
		lua_setglobal(L, binding.GlobalName.c_str());
		//lua_
		return 1;
	}

	static int Binding_New(lua_State* L)
	{
		Binding binding;
		std::string globalName = luaL_checkstring(L, 1);
		binding.GlobalName = globalName;
		bool isInt;
		// check for edit type first so we know if we are reading an int or float
		if (lua_isstring(L, 3))
		{
			// edit type
			const char* editType = luaL_checkstring(L, 3);
			if (strcmp(editType, "InputFloat") == 0)
				binding.editType = Binding::EditType::InputFloat;
			else if (strcmp(editType, "DragFloat") == 0)
				binding.editType = Binding::EditType::DragFloat;
			else if (strcmp(editType, "SliderFloat") == 0)
				binding.editType = Binding::EditType::SliderFloat;
			else if (strcmp(editType, "InputInt") == 0)
				binding.editType = Binding::EditType::InputInt;
			else if (strcmp(editType, "DragInt") == 0)
				binding.editType = Binding::EditType::DragInt;
			else if (strcmp(editType, "SliderInt") == 0)
				binding.editType = Binding::EditType::SliderInt;
			else if (strcmp(editType, "ToggleBox") == 0)
				binding.editType = Binding::EditType::ToggleBox;
			else if (strcmp(editType, "ToggleButton") == 0)
				binding.editType = Binding::EditType::ToggleButton;
			isInt = !(strcmp(editType, "InputFloat") == 0 || strcmp(editType, "DragFloat") == 0 || strcmp(editType, "SliderFloat") == 0);

			(isInt) ? binding.MinValue.i = 0: binding.MinValue.f = 0.0f;
			(isInt) ? binding.MaxValue.i = 1 : binding.MaxValue.f = 1.0f;
			// min max
			if (lua_isnumber(L, 4))
			{
				float minValue = luaL_checknumber(L, 4);
				(isInt) ? binding.MinValue.i = (int)minValue : binding.MinValue.f = minValue;
			}
			if (lua_isnumber(L, 5))
			{
				float maxValue = luaL_checknumber(L, 5);
				(isInt) ? binding.MaxValue.i = (int)maxValue : binding.MaxValue.f = maxValue;
			}
		}

		if (lua_isnumber(L, 2))
		{
			lua_Number startValue = luaL_checknumber(L, 2);
			(isInt) ? binding.Set((int)startValue) : binding.Set((float)startValue);
		}
		else if (lua_isboolean(L, 2))
		{
			bool startValue = lua_toboolean(L, 2);
			binding.Set(startValue);
		}

		if (lua_isnumber(L, 3))
		{
			luaL_checknumber(L, 3);
		}


		ScriptBinding::GetInstance().Bind(binding);

		//lua_Debug info;
		//int level = 0;
		//while (lua_getstack(L, level, &info)) {
		//	//lua_getinfo(L, "nSl", &info);
		//	//fprintf(stderr, "  [%d] %s:%d -- %s [%s]\n",
		//	//	level, info.short_src, info.currentline,
		//	//	(info.name ? info.name : "<unknown>"), info.what);
		//	const char* blu = lua_getlocal(L, &info, 1);
		//	if (blu == nullptr)
		//		std::cout << "null\n";
		//	else
		//	{
		//		std::string des = blu;
		//		std::cout << "tes: " << des.c_str() << std::endl;
		//	}
		//	++level;
		//}
		//std::cout << "yatta" << ar.name << std::endl;
		//const float n = static_cast<float>(luaL_checknumber(L, 1));

		//lua_pushbinding(L, n);
		return 1;
	}

	//static int Binding___add(lua_State* L)
	//{
	//	Vector& a = *lua_checkvector(L, 1);
	//	Vector& b = *lua_checkvector(L, 2);
	//
	//	lua_pushvector(L, a.x + b.x, a.y + b.y);
	//
	//	return 1;
	//}
	//
	//static int Binding___sub(lua_State* L)
	//{
	//	const Vector& a = *lua_checkvector(L, 1);
	//	const Vector& b = *lua_checkvector(L, 2);
	//
	//	lua_pushvector(L, a.x - b.x, a.y - b.y);
	//	return 1;
	//}
	//
	//
	//static int Binding___tostring(lua_State* L)
	//{
	//	Binding& binding = *lua_checkbinding(L, 1);
	//	char Buffer[256];
	//	//sprintf_s(Buffer, "(%f, %f)", binding.x, LuaVector.y);
	//	sprintf_s(Buffer, "%f", binding.number);
	//	lua_pushstring(L, Buffer);
	//	return 1;
	//}
	//
	//
	//static int Binding___eq(lua_State* L)
	//{
	//	Binding& a = *lua_checkbinding(L, 1);
	//	const lua_Number value = luaL_checknumber(L, 2);
	//	std::cout << "value is: " << a.number << std::endl;
	//	lua_pushboolean(L, (value == a.number));
	//	return 1;
	//}

	const luaL_Reg Binding_Functions[] =
	{
		{ "Bind", Binding_New },
		{ NULL, NULL }
	};

	inline void Generate_Binding_Methods(lua_State* L)
	{
		//lua_pushcfunction(L, Binding___tostring); lua_setfield(L, -2, "__tostring");
		//lua_pushcfunction(L, Binding___add); lua_setfield(L, -2, "__add");
		//lua_pushcfunction(L, Binding___sub); lua_setfield(L, -2, "__sub");
		//lua_pushcfunction(L, Binding___eq); lua_setfield(L, -2, "__eq");
		//lua_pushcfunction(L, Binding___unm); lua_setfield(L, -2, "__unm");
		lua_pushvalue(L, -1); lua_setfield(L, -2, "__index");

		//lua_pushcfunction(L, Binding_x); lua_setfield(L, -2, "x");
	};

	int luaopen_editor(lua_State* L)
	{
		//lua_register(L, tableName, Binding_New);
		//luaL_newmetatable(L, tableName);
		//Generate_Binding_Methods(L);


		//lua_newtable(L);
		lua_register(L, "Bind", Binding_New);
		//lua_pushcfunction(L, Binding_New); lua_setfield(L, -2, "Bindu");
		//lua_setglobal(L, tableName);
		// create it as metatable
		//luaL_newmetatable(L, "Bind");


		return 1;
	}

	Binding* lua_checkbinding(lua_State* L, int n)
	{
		Binding* UserData = static_cast<Binding*>(luaL_checkudata(L, n, tableName));
		char Error[128];
		sprintf_s(Error, "'%s' expected", tableName);
		luaL_argcheck(L, UserData != nullptr, n, Error);
		return UserData;
	}

	//Vector lua_checkvectorEx(lua_State* L, int n)
	//{
	//	if (lua_isnumber(L, n))
	//	{
	//		lua_Number xPos = luaL_checknumber(L, n);
	//		lua_Number yPos = luaL_checknumber(L, n + 1);
	//		return Vector(xPos, yPos);
	//	}
	//	Vector* vector = lua_checkvector(L, n);
	//	return *vector;
	//}

	int lua_pushbinding(lua_State* L, float n)
	{
		//void* UserData = lua_newuserdata(L, sizeof(Binding));
		////luaL_setmetatable(L, tableName);
		//luaL_getmetatable(L, tableName);
		//lua_setmetatable(L, -2);
		//
		//Binding* binding = static_cast<Binding*>(UserData);
		//binding->number = n;
		//ScriptBinding::GetInstance().Bind(binding);

		return 1;
	}
}
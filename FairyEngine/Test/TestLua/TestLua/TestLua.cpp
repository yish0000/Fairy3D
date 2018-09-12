// TestLua.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

extern "C" {
	#include "lua.h"
	#include "lauxlib.h"
	#include "lualib.h"
}

static int counter(lua_State* L);

int newCounter(lua_State* L)
{
	lua_pushnumber(L, 0);
	lua_pushcclosure(L, counter, 1);
	return 1;
}

static int counter(lua_State* L)
{
	double val = lua_tonumber(L, lua_upvalueindex(1));
	lua_pushnumber(L, ++val);
	lua_pushvalue(L, -1);
	lua_replace(L, lua_upvalueindex(1));
	return 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	lua_State* pLua = luaL_newstate();
	luaL_openlibs(pLua);

	const luaL_reg regs[] =
	{
		{ "newCounter", newCounter },
		{ "counter", counter },
		{ NULL, NULL },
	};
	luaL_register(pLua, "test", regs);

	if (luaL_loadfile(pLua, "./test.lua"))
	{
		const char* err = lua_tostring(pLua, -1);
		printf("%s\n", err);
		getchar();
		return 0;
	}

	lua_pcall(pLua, 0, 0, 0);

	lua_close(pLua);
	getchar();
	return 0;
}


// TestLua.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <regex>

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

static int panic_func(lua_State *L)
{
	printf("unprotected error in call to Lua API (%s)", lua_tostring(L, -1));
	return 0;
}

static int error_func(lua_State *L)
{
	if (!lua_isstring(L, 1))
		return 1;

	lua_getfield(L, LUA_GLOBALSINDEX, "debug");
	if (!lua_istable(L, -1))
	{
		lua_pop(L, 1);
		return 1;
	}

	lua_getfield(L, -1, "traceback");
	if (!lua_isfunction(L, -1))
	{
		lua_pop(L, 2);
		return 1;
	}

	lua_pushvalue(L, 1);
	lua_pushinteger(L, 2);
	lua_call(L, 2, 1);
	return 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	lua_atpanic(L, panic_func);

	const luaL_reg regs[] =
	{
		{ "newCounter", newCounter },
		{ "counter", counter },
		{ NULL, NULL },
	};
	luaL_register(L, "test", regs);

	if (luaL_loadfile(L, "./test.lua"))
	{
		const char* err = lua_tostring(L, -1);
		printf("%s\n", err);
		getchar();
		return 0;
	}

	int oldTop = lua_gettop(L) - 1;
	lua_pushcfunction(L, error_func);
	lua_insert(L, oldTop + 1);
	int ret = lua_pcall(L, 0, 0, oldTop + 1);
	lua_remove(L, oldTop + 1);

	if (ret != 0)
		printf(lua_tostring(L, -1));

	lua_close(L);
	getchar();
	return 0;
}


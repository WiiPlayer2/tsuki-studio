#include "lua_helper.h"
#include "lua_obs.h"
#include "lua_gs.h"
#include "lua_obstypes.h"
#include "obs.h"

void lua_print_error(lua_State* L)
{
	const char* message = lua_tostring(L, -1);

	blog(LOG_DEBUG, "[LUA::ERROR] %s", message);
	lua_pop(L, 2);
}

void lua_print_table(lua_State* L, int idx)
{
	lua_pushvalue(L, idx);
	lua_pushnil(L);
	while (lua_next(L, -2))
	{
		lua_pushvalue(L, -2);
		const char* key = luaL_tolstring(L, -1, NULL);
		lua_pop(L, 1);
		const char* val = luaL_tolstring(L, -2, NULL);
		blog(LOG_DEBUG, "%s => %s", key, val);
		lua_pop(L, 3);
	}
	lua_pop(L, 1);
}

int lua_bfree(lua_State* L)
{
	int args;
	lua_failargs(L, 1, args);
	lua_failtype(L, 1, LUA_TUSERDATA);

	void* ptr = lua_to_cpointer(L, 1);

	bfree(ptr);

	return 0;
}

int lua_notnull(lua_State* L)
{
	int args;
	lua_failargs(L, 1, args);
	lua_failtype(L, 1, LUA_TUSERDATA);

	void* ptr = lua_to_cpointer(L, 1);

	bool ret = ptr;

	lua_pushboolean(L, ret);
	return 1;
}

const luaL_Reg helperlib[] = {
	{ "bfree", lua_bfree },
	{ "notnull", lua_notnull },
	{ NULL, NULL },
};

int luaopen_helperlib(lua_State *L)
{
	luaL_newlib(L, helperlib);
	return 1;
}

void lua_open_helper(lua_State* L)
{
	luaL_requiref(L, "helper", luaopen_helperlib, 1);
	lua_pop(L, 1);
}

lua_State* newLstate()
{
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);
	lua_open_obs(L);
	lua_open_gs(L);
	lua_open_vec2(L);
	lua_open_vec4(L);
	//lua_open_obsmodule(L);
	lua_open_helper(L);
	return L;
}
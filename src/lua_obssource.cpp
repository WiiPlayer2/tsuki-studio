#include "lua_obssource.h"
#include "lua_helper.h"
#include "obs.h"

#define lua_ld(L, k) lua_pushstring(L, k); lua_gettable(L, 1);

int lua_obssource_register(lua_State* L)
{
	int args;
	lua_failargs(L, 1, args);

	obs_source_info* f = new obs_source_info();

	//TODO: Cast and pass source info
	lua_ld(L, "id");
	f->id = lua_tostring(L, lua_gettop(L));
	lua_ld(L, "type");
	f->type = (obs_source_type)lua_tointeger(L, lua_gettop(L));
	lua_ld(L, "output_flags");
	f->output_flags = lua_tointeger(L, lua_gettop(L));


	obs_register_source(f);
	return 0;
}

const luaL_Reg obssourcelib[] = {
	{ "register", lua_obssource_register },
	{ NULL, NULL },
};

int luaopen_obssourcelib(lua_State* L)
{
	luaL_newlib(L, obssourcelib);
	return 1;
}

void lua_open_obssource(lua_State* L)
{
	luaL_requiref(L, "obssource", luaopen_obssourcelib, 1);
	lua_pop(L, 1);
}
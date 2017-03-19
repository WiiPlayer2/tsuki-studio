#include "lua_gs.h"
#include "lua_helper.h"
#include "lua_obstypes.h"
#include "obs.h"

#define lconst(L, k, v) lua_pushnumber(L, v); lua_setfield(L, -2, k);

int lua_gs_effect_create_from_file(lua_State* L)
{
	int args;
	lua_failargs(L, 1, args);
	lua_failtype(L, 1, LUA_TSTRING);

	const char* file = lua_tostring(L, 1);
	char *error = NULL;

	gs_effect_t* effect = gs_effect_create_from_file(file, &error);

	lua_push_cpointer(L, effect);
	lua_pushstring(L, error);
	return 2;
}

int lua_gs_effect_destroy(lua_State* L)
{
	int args;
	lua_failargs(L, 1, args);
	lua_failtype(L, 1, LUA_TUSERDATA);

	gs_effect_t* effect = (gs_effect_t*)lua_to_cpointer(L, 1);

	gs_effect_destroy(effect);

	return 0;
}

int lua_gs_effect_get_param_by_name(lua_State* L)
{
	int args;
	lua_failargs(L, 2, args);
	lua_failtype(L, 1, LUA_TUSERDATA);
	lua_failtype(L, 2, LUA_TSTRING);

	gs_effect_t* effect = (gs_effect_t*)lua_to_cpointer(L, 1);
	const char* name = lua_tostring(L, 2);

	gs_eparam_t* param = gs_effect_get_param_by_name(effect, name);

	lua_push_cpointer(L, param);
	return 1;
}

int lua_gs_effect_set_vec4(lua_State *L)
{
	int args;
	lua_failargs(L, 2, args);
	lua_failtype(L, 1, LUA_TUSERDATA);
	lua_failtype(L, 2, LUA_TUSERDATA);

	gs_eparam_t* param = (gs_eparam_t*)lua_to_cpointer(L, 1);
	vec4* val = lua_to_vec4(L, 2);

	gs_effect_set_vec4(param, val);

	return 0;
}

int lua_gs_effect_set_vec2(lua_State *L)
{
	int args;
	lua_failargs(L, 2, args);
	lua_failtype(L, 1, LUA_TUSERDATA);
	lua_failtype(L, 2, LUA_TUSERDATA);

	gs_eparam_t* param = (gs_eparam_t*)lua_to_cpointer(L, 1);
	vec2* val = lua_to_vec2(L, 2);

	gs_effect_set_vec2(param, val);

	return 0;
}

int lua_gs_effect_set_bool(lua_State *L)
{
	int args;
	lua_failargs(L, 2, args);
	lua_failtype(L, 1, LUA_TUSERDATA);
	lua_failtype(L, 2, LUA_TBOOLEAN);

	gs_eparam_t* param = (gs_eparam_t*)lua_to_cpointer(L, 1);
	bool val = lua_toboolean(L, 2);

	gs_effect_set_bool(param, val);

	return 0;
}

int lua_vec2_set(lua_State* L)
{
	int args;
	lua_failargs(L, 3, args);
	lua_failtype(L, 1, LUA_TUSERDATA);
	lua_failtype(L, 2, LUA_TNUMBER);
	lua_failtype(L, 3, LUA_TNUMBER);

	vec2* v = lua_to_vec2(L, 1);
	float x = lua_tonumber(L, 2);
	float y = lua_tonumber(L, 3);

	vec2_set(v, x, y);

	return 0;
}

int lua_vec4_from_rgba(lua_State* L)
{
	int args;
	lua_failargs(L, 2, args);
	lua_failtype(L, 1, LUA_TUSERDATA);
	lua_failtype(L, 2, LUA_TNUMBER);

	vec4* v = lua_to_vec4(L, 1);
	uint32_t rgba = lua_tointeger(L, 2);

	vec4_from_rgba(v, rgba);

	return 0;
}

int lua_vec2_zero(lua_State* L)
{
	int args;
	lua_failargs(L, 1, args);
	lua_failtype(L, 1, LUA_TUSERDATA);

	vec2* v = lua_to_vec2(L, 1);

	vec2_zero(v);

	return 0;
}

const luaL_Reg gslib[] = {
	{ "effect_create_from_file", lua_gs_effect_create_from_file },
	{ "effect_destroy", lua_gs_effect_destroy },
	{ "effect_get_param_by_name", lua_gs_effect_get_param_by_name },
	{ "effect_set_vec4", lua_gs_effect_set_vec4 },
	{ "effect_set_vec2", lua_gs_effect_set_vec2 },
	{ "effect_set_bool", lua_gs_effect_set_bool },
	{ "vec2_set", lua_vec2_set },
	{ "vec4_from_rgba", lua_vec4_from_rgba },
	{ "vec2_zero", lua_vec2_zero },
	{ "RGBA", NULL },
	{ NULL, NULL }
};

int luaopen_gslib(lua_State *L)
{
	luaL_newlib(L, gslib);
	lconst(L, "RGBA", GS_RGBA);
	return 1;
}

void lua_open_gs(lua_State* L)
{
	luaL_requiref(L, "gs", luaopen_gslib, 1);
	lua_pop(L, 1);
}
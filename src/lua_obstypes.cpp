#include "lua_obstypes.h"
#include "lua.hpp"
#include "obs.h"
#include "lua_helper.h"

#define ld(L, k, idx) lua_pushstring(L, k); lua_gettable(L, idx);
#define sto_f(L, k, idx, f) ld(L, k, idx); f = luaL_ref(L, LUA_REGISTRYINDEX);

void lua_push_obs_source_info(lua_State* L, lua_obs_source_info v)
{

}

lua_obs_source_info lua_to_obs_source_info(lua_State* L, int idx)
{
	lua_cfailtype(L, idx, LUA_TTABLE);

	lua_obs_source_info ret;

	//lua_print_table(L, idx);

	sto_f(L, "create", idx, ret.create_ref);
	sto_f(L, "get_properties", idx, ret.get_properties_ref);
	sto_f(L, "destroy", idx, ret.destroy_ref);
	sto_f(L, "update", idx, ret.update_ref);
	sto_f(L, "video_render", idx, ret.video_render_ref);
	sto_f(L, "get_width", idx, ret.get_width_ref);
	sto_f(L, "get_height", idx, ret.get_height_ref);
	sto_f(L, "video_tick", idx, ret.video_tick_ref);

	return ret;
}

void lua_push_cpointer(lua_State* L, void* ptr)
{
	void **bp = (void**)lua_newuserdata(L, sizeof(ptr));
	*bp = ptr;
}

void* lua_to_cpointer(lua_State* L, int idx)
{
	lua_cfailtype(L, idx, LUA_TUSERDATA);
	void **bp = (void**)lua_touserdata(L, idx);
	return *bp;
}

#pragma region vec2 userdata
int lua_vec2_new(lua_State* L)
{
	vec2* v = (vec2*)lua_newuserdata(L, sizeof(vec2));
	luaL_getmetatable(L, "vec2");
	lua_setmetatable(L, -2);
	return 1;
}

int lua_vec2_x(lua_State* L)
{
	int args = lua_gettop(L);

	vec2* v = lua_to_vec2(L, 1);

	if (args == 2)
	{
		v->x = luaL_checknumber(L, 2);
	}
	else if (args != 1)
	{
		lua_throw(L, "invalid argument count");
	}

	lua_pushnumber(L, v->x);
	return 1;
}

int lua_vec2_y(lua_State* L)
{
	int args = lua_gettop(L);

	vec2* v = lua_to_vec2(L, 1);

	if (args == 2)
	{
		v->y = luaL_checknumber(L, 2);
	}
	else if (args != 1)
	{
		lua_throw(L, "invalid argument count");
	}

	lua_pushnumber(L, v->y);
	return 1;
}

vec2* lua_to_vec2(lua_State* L, int idx)
{
	luaL_checktype(L, idx, LUA_TUSERDATA);
	vec2* v = (vec2*)lua_touserdata(L, idx);
	return v;
}

const luaL_Reg vec2_methods[] =
{
	{ "new", lua_vec2_new },
	{ "x", lua_vec2_x },
	{ "y", lua_vec2_y },
	{ NULL, NULL },
};

int lua_register_vec2(lua_State* L)
{
	luaL_newlib(L, vec2_methods);

	luaL_newmetatable(L, "vec2");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	luaL_setfuncs(L, vec2_methods, 0);

	lua_pop(L, 1);
	return 1;
}

void lua_open_vec2(lua_State* L)
{
	luaL_requiref(L, "vec2", lua_register_vec2, 1);
	lua_pop(L, 1);
}
#pragma endregion

#pragma region vec4 userdata
int lua_vec4_new(lua_State* L)
{
	vec4* v = (vec4*)lua_newuserdata(L, sizeof(vec4));
	luaL_getmetatable(L, "vec4");
	lua_setmetatable(L, -2);
	return 1;
}

int lua_vec4_x(lua_State* L)
{
	int args = lua_gettop(L);

	vec4* v = lua_to_vec4(L, 1);

	if (args == 2)
	{
		v->x = luaL_checknumber(L, 2);
	}
	else if (args != 1)
	{
		lua_throw(L, "invalid argument count");
	}

	lua_pushnumber(L, v->x);
	return 1;
}

int lua_vec4_y(lua_State* L)
{
	int args = lua_gettop(L);

	vec4* v = lua_to_vec4(L, 1);

	if (args == 2)
	{
		v->y = luaL_checknumber(L, 2);
	}
	else if (args != 1)
	{
		lua_throw(L, "invalid argument count");
	}

	lua_pushnumber(L, v->y);
	return 1;
}

int lua_vec4_z(lua_State* L)
{
	int args = lua_gettop(L);

	vec4* v = lua_to_vec4(L, 1);

	if (args == 2)
	{
		v->z = luaL_checknumber(L, 2);
	}
	else if (args != 1)
	{
		lua_throw(L, "invalid argument count");
	}

	lua_pushnumber(L, v->z);
	return 1;
}

int lua_vec4_w(lua_State* L)
{
	int args = lua_gettop(L);

	vec4* v = lua_to_vec4(L, 1);

	if (args == 2)
	{
		v->w = luaL_checknumber(L, 2);
	}
	else if (args != 1)
	{
		lua_throw(L, "invalid argument count");
	}

	lua_pushnumber(L, v->w);
	return 1;
}

vec4* lua_to_vec4(lua_State* L, int idx)
{
	luaL_checktype(L, idx, LUA_TUSERDATA);
	vec4* v = (vec4*)lua_touserdata(L, idx);
	return v;
}

const luaL_Reg vec4_methods[] =
{
	{ "new", lua_vec4_new },
	{ "x", lua_vec4_x },
	{ "y", lua_vec4_y },
	{ "z", lua_vec4_z },
	{ "w", lua_vec4_w },
	{ NULL, NULL },
};

int lua_register_vec4(lua_State* L)
{
	luaL_newlib(L, vec4_methods);

	luaL_newmetatable(L, "vec4");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");

	luaL_setfuncs(L, vec4_methods, 0);

	lua_pop(L, 1);
	return 1;
}

void lua_open_vec4(lua_State* L)
{
	luaL_requiref(L, "vec4", lua_register_vec4, 1);
	lua_pop(L, 1);
}
#pragma endregion


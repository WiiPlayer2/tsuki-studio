#include "lua_obs.h"
#include "lua_helper.h"
#include "lua_obstypes.h"
#include "obs.h"
#include "tsuki-studio.h"

#define lconst(L, k, v) lua_pushnumber(L, v); lua_setfield(L, -2, k);

int lua_obs_log(lua_State* L)
{
	int args;
	lua_failargs(L, 2, args);
	lua_failtype(L, 1, LUA_TNUMBER);
	lua_failtype(L, 2, LUA_TSTRING);

	blog(lua_tointeger(L, 1), lua_tostring(L, 2));

	return 0;
}

#pragma region property
int lua_obs_property_list_add_int(lua_State* L)
{
	int args;
	lua_failargs(L, 3, args);
	lua_failtype(L, 1, LUA_TUSERDATA);
	lua_failtype(L, 2, LUA_TSTRING);
	lua_failtype(L, 3, LUA_TNUMBER);

	obs_property_t* prop = (obs_property_t*)lua_to_cpointer(L, 1);
	const char* name = lua_tostring(L, 2);
	long long val = lua_tointeger(L, 3);

	size_t size = obs_property_list_add_int(prop, name, val);

	lua_pushinteger(L, size);
	return 1;
}
#pragma endregion

#pragma region properties
int lua_obs_properties_add_list(lua_State* L)
{
	int args;
	lua_failargs(L, 5, args);
	lua_failtype(L, 1, LUA_TUSERDATA);
	lua_failtype(L, 2, LUA_TSTRING);
	lua_failtype(L, 3, LUA_TSTRING);
	lua_failtype(L, 4, LUA_TNUMBER);
	lua_failtype(L, 5, LUA_TNUMBER);

	obs_properties_t* props = (obs_properties_t*)lua_to_cpointer(L, 1);
	const char* name = lua_tostring(L, 2);
	const char* desc = lua_tostring(L, 3);
	obs_combo_type combo_type = (obs_combo_type)lua_tointeger(L, 4);
	obs_combo_format combo_format = (obs_combo_format)lua_tointeger(L, 5);

	obs_property_t* prop = obs_properties_add_list(props, name, desc, combo_type, combo_format);

	lua_push_cpointer(L, prop);

	return 1;
}

int lua_obs_properties_add_color(lua_State* L)
{
	int args;
	lua_failargs(L, 3, args);
	lua_failtype(L, 1, LUA_TUSERDATA);
	lua_failtype(L, 2, LUA_TSTRING);
	lua_failtype(L, 3, LUA_TSTRING);

	obs_properties_t* props = (obs_properties_t*)lua_to_cpointer(L, 1);
	const char* name = lua_tostring(L, 2);
	const char* desc = lua_tostring(L, 3);

	obs_property_t* prop = obs_properties_add_color(props, name, desc);

	lua_push_cpointer(L, prop);

	return 1;
}

int lua_obs_properties_add_int(lua_State* L)
{
	int args;
	lua_failargs(L, 6, args);
	lua_failtype(L, 1, LUA_TUSERDATA);
	lua_failtype(L, 2, LUA_TSTRING);
	lua_failtype(L, 3, LUA_TSTRING);
	lua_failtype(L, 4, LUA_TNUMBER);
	lua_failtype(L, 5, LUA_TNUMBER);
	lua_failtype(L, 6, LUA_TNUMBER);

	obs_properties_t* props = (obs_properties_t*)lua_to_cpointer(L, 1);
	const char* name = lua_tostring(L, 2);
	const char* desc = lua_tostring(L, 3);
	int min = lua_tointeger(L, 4);
	int max = lua_tointeger(L, 5);
	int step = lua_tointeger(L, 6);

	obs_property_t* prop = obs_properties_add_int(props, name, desc, min, max, step);

	lua_push_cpointer(L, prop);

	return 1;
}

int lua_obs_properties_add_bool(lua_State* L)
{
	int args;
	lua_failargs(L, 3, args);
	lua_failtype(L, 1, LUA_TUSERDATA);
	lua_failtype(L, 2, LUA_TSTRING);
	lua_failtype(L, 3, LUA_TSTRING);

	obs_properties_t* props = (obs_properties_t*)lua_to_cpointer(L, 1);
	const char* name = lua_tostring(L, 2);
	const char* desc = lua_tostring(L, 3);

	obs_property_t* prop = obs_properties_add_bool(props, name, desc);

	lua_push_cpointer(L, prop);

	return 1;
}
#pragma endregion

#pragma region data
int lua_obs_data_get_int(lua_State* L)
{
	int args;
	lua_failargs(L, 2, args);
	lua_failtype(L, 1, LUA_TUSERDATA);
	lua_failtype(L, 2, LUA_TSTRING);

	obs_data_t* data = (obs_data_t*)lua_to_cpointer(L, 1);
	const char* name = lua_tostring(L, 2);

	long long ret = obs_data_get_int(data, name);

	lua_pushinteger(L, ret);
	return 1;
}

int lua_obs_data_get_bool(lua_State* L)
{
	int args;
	lua_failargs(L, 2, args);
	lua_failtype(L, 1, LUA_TUSERDATA);
	lua_failtype(L, 2, LUA_TSTRING);

	obs_data_t* data = (obs_data_t*)lua_to_cpointer(L, 1);
	const char* name = lua_tostring(L, 2);

	bool ret = obs_data_get_bool(data, name);

	lua_pushboolean(L, ret);
	return 1;
}
#pragma endregion

int lua_obs_enter_graphics(lua_State* L)
{
	int args;
	lua_failargs(L, 0, args);

	obs_enter_graphics();

	return 0;
}

int lua_obs_leave_graphics(lua_State* L)
{
	int args;
	lua_failargs(L, 0, args);

	obs_leave_graphics();

	return 0;
}

#pragma region source
int lua_obs_source_process_filter_begin(lua_State *L)
{
	int args;
	lua_failargs(L, 3, args);
	lua_failtype(L, 1, LUA_TUSERDATA);
	lua_failtype(L, 2, LUA_TNUMBER);
	lua_failtype(L, 3, LUA_TNUMBER);

	obs_source_t* context = (obs_source_t*)lua_to_cpointer(L, 1);
	gs_color_format format = (gs_color_format)lua_tointeger(L, 2);
	obs_allow_direct_render direct = (obs_allow_direct_render)lua_tointeger(L, 3);

	bool ret = obs_source_process_filter_begin(context, format, direct);

	lua_pushboolean(L, ret);
	return 1;
}

int lua_obs_source_process_filter_end(lua_State *L)
{
	int args;
	lua_failargs(L, 4, args);
	lua_failtype(L, 1, LUA_TUSERDATA);
	lua_failtype(L, 2, LUA_TUSERDATA);
	lua_failtype(L, 3, LUA_TNUMBER);
	lua_failtype(L, 4, LUA_TNUMBER);

	obs_source_t* context = (obs_source_t*)lua_to_cpointer(L, 1);
	gs_effect_t* effect = (gs_effect_t*)lua_to_cpointer(L, 2);
	uint32_t width = lua_tointeger(L, 3);
	uint32_t height = lua_tointeger(L, 4);

	obs_source_process_filter_end(context, effect, width, height);

	return 0;
}

int lua_obs_source_get_base_width(lua_State *L)
{
	int args;
	lua_failargs(L, 1, args);
	lua_failtype(L, 1, LUA_TUSERDATA);

	obs_source_t* source = (obs_source_t*)lua_to_cpointer(L, 1);

	uint32_t ret = obs_source_get_base_width(source);

	lua_pushinteger(L, ret);
	return 1;
}

int lua_obs_source_get_base_height(lua_State *L)
{
	int args;
	lua_failargs(L, 1, args);
	lua_failtype(L, 1, LUA_TUSERDATA);

	obs_source_t* source = (obs_source_t*)lua_to_cpointer(L, 1);

	uint32_t ret = obs_source_get_base_height(source);

	lua_pushinteger(L, ret);
	return 1;
}
#pragma endregion

#pragma region filter
int lua_obs_filter_get_target(lua_State *L)
{
	int args;
	lua_failargs(L, 1, args);
	lua_failtype(L, 1, LUA_TUSERDATA);

	obs_source_t *context = (obs_source_t*)lua_to_cpointer(L, 1);

	obs_source_t* target = obs_filter_get_target(context);

	lua_push_cpointer(L, target);
	return 1;
}
#pragma endregion

const luaL_Reg obslib[] = {
#pragma region property
	{ "property_list_add_int", lua_obs_property_list_add_int },
#pragma endregion
#pragma region properties
	{ "properties_add_color", lua_obs_properties_add_color },
	{ "properties_add_int", lua_obs_properties_add_int },
	{ "properties_add_bool", lua_obs_properties_add_bool },
	{ "properties_add_list", lua_obs_properties_add_list },
#pragma endregion
#pragma region data
	{ "data_get_int", lua_obs_data_get_int },
	{ "data_get_bool", lua_obs_data_get_bool },
#pragma endregion
	{ "enter_graphics", lua_obs_enter_graphics },
	{ "leave_graphics", lua_obs_leave_graphics },
#pragma region source
	{ "source_process_filter_begin", lua_obs_source_process_filter_begin },
	{ "source_process_filter_end", lua_obs_source_process_filter_end },
	{ "source_get_base_width", lua_obs_source_get_base_width },
	{ "source_get_base_height", lua_obs_source_get_base_height },
#pragma endregion
#pragma region filter
	{ "filter_get_target", lua_obs_filter_get_target },
#pragma endregion
	{ "COMBO_TYPE_LIST", NULL },
	{ "COMBO_FORMAT_INT", NULL },
	{ "NO_DIRECT_RENDERING", NULL },
	{ "log", lua_obs_log },
	{ "LOG_DEBUG", NULL },
	{ "LOG_INFO", NULL },
	{ "LOG_WARNING", NULL },
	{ "LOG_ERROR", NULL },
	{ NULL, NULL }
};

int luaopen_obslib(lua_State *L)
{
	luaL_newlib(L, obslib);
	lconst(L, "COMBO_TYPE_LIST", OBS_COMBO_TYPE_LIST);
	lconst(L, "COMBO_FORMAT_INT", OBS_COMBO_FORMAT_INT);
	lconst(L, "NO_DIRECT_RENDERING", OBS_NO_DIRECT_RENDERING);
	lconst(L, "LOG_DEBUG", LOG_DEBUG);
	lconst(L, "LOG_INFO", LOG_INFO);
	lconst(L, "LOG_WARNING", LOG_WARNING);
	lconst(L, "LOG_ERROR", LOG_ERROR);
	return 1;
}

void lua_open_obs(lua_State* L)
{
	luaL_requiref(L, "obs", luaopen_obslib, 1);
	lua_pop(L, 1);
}
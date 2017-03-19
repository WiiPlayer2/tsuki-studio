#pragma once
#include "lua.hpp"
#include "obs.h"

struct lua_obs_source_info
{
	int create_ref;
	int get_properties_ref;
	int destroy_ref;
	int update_ref;
	int video_render_ref;
	int get_width_ref;
	int get_height_ref;
	int video_tick_ref;
};

void lua_push_obs_source_info(lua_State* L, lua_obs_source_info v);
lua_obs_source_info lua_to_obs_source_info(lua_State* L, int idx);

void lua_push_cpointer(lua_State* L, void* ptr);
void* lua_to_cpointer(lua_State* L, int idx);

vec2* lua_to_vec2(lua_State* L, int idx);
void lua_open_vec2(lua_State* L);

vec4* lua_to_vec4(lua_State* L, int idx);
void lua_open_vec4(lua_State* L);
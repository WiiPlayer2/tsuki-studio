#include "lua-video-filter.h"
#include "lua.hpp"
#include "lua_helper.h"
#include "lua_obstypes.h"

#define SETTING_LUA_FILE_PATH		"LUA_FILE_PATH"
#define SETTING_LUA_RELOAD_BUTTON	"LUA_RELOAD_BUTTON"
#define TEXT_LUA_FILE_PATH			"Lua file"
#define TEXT_LUA_RELOAD_BUTTON		"Reload lua file"

#define check_lua(data) if(!data->L) { lua_load(data); }
#define has_cb(data, cb) (data->L && data->source_info.cb != -1)
#define lua_cb(data, cb) check_lua(data) if(has_cb(data, cb))
#define lua_dpcall(L, n, r, f, res) res = lua_pcall(L, n, r, f); if(res != LUA_OK) { lua_print_error(L); }
#define pushdata(f) if(f->data_ref == -1) { lua_pushnil(f->L); } else { lua_rawgeti(f->L, LUA_REGISTRYINDEX, f->data_ref); }

struct filter_data
{
	obs_source_t* context;
	obs_data_t* settings;

	const char* lua_file_path;
	lua_State* L;
	int data_ref = -1;

	lua_obs_source_info source_info;
};

void lua_load(filter_data* filter)
{
	filter->L = newLstate();
	bool failed = true;
	int result = luaL_loadfile(filter->L, filter->lua_file_path);
	if (result == LUA_OK)
	{
		result = lua_pcall(filter->L, 0, 1, 0);
		if (result == LUA_OK)
		{
			int rets = lua_gettop(filter->L);
			if (rets == 1 && lua_type(filter->L, 1) == LUA_TTABLE)
			{
				filter->source_info = lua_to_obs_source_info(filter->L, 1);
				failed = false;

				//filter_create
				lua_ld_cb(filter->L, filter->source_info.create_ref);
				lua_push_cpointer(filter->L, filter->settings);
				lua_push_cpointer(filter->L, filter->context);
				int res;
				lua_dpcall(filter->L, 2, 2, 0, res);
				lua_pushvalue(filter->L, 2);
				filter->data_ref = luaL_ref(filter->L, LUA_REGISTRYINDEX);
			}
			else
			{
				blog(LOG_ERROR, "[LUA::ERROR] Script return values are wrong");
				for (int i = 1; i <= rets; i++)
				{
					const char* v = luaL_tolstring(filter->L, i, NULL);
					blog(LOG_ERROR, "[LUA::ERROR] Return value #%i: %s", i, v);
					lua_pop(filter->L, 1);

					if (lua_type(filter->L, i) == LUA_TTABLE)
					{
						lua_print_table(filter->L, i);
					}
				}
			}
		}
		else
		{
			lua_print_error(filter->L);
		}
	}
	else
	{
		lua_print_error(filter->L);
	}

	if (failed)
	{
		filter->source_info = { 0 };
		filter->data_ref = -1;
		lua_close(filter->L);
		filter->L = NULL;
	}
}

const char* filter_get_name(void *unused)
{
	return "Lua Video Filter";
}

void filter_update(void* data, obs_data_t* settings)
{
	filter_data* filter = (filter_data*)data;

	filter->lua_file_path = obs_data_get_string(settings, SETTING_LUA_FILE_PATH);

	lua_cb(filter, update_ref)
	{
		lua_ld_cb(filter->L, filter->source_info.update_ref);
		pushdata(filter);
		lua_push_cpointer(filter->L, settings);
		int res;
		lua_dpcall(filter->L, 2, 0, 0, res);
	}
}

void* filter_create(obs_data_t *settings, obs_source_t *context)
{
	filter_data* filter = (filter_data*)bzalloc(sizeof(filter_data));
	filter->context = context;
	filter->settings = settings;
	filter->lua_file_path = obs_data_get_string(settings, SETTING_LUA_FILE_PATH);

	lua_cb(filter, create_ref) { }

	return filter;
}

void filter_destroy(void *data)
{
	filter_data* filter = (filter_data*)data;

	lua_cb(filter, destroy_ref)
	{
		lua_ld_cb(filter->L, filter->source_info.destroy_ref);
		pushdata(filter);
		int res;
		lua_dpcall(filter->L, 1, 0, 0, res);
	}

	if (filter->L)
	{
		lua_close(filter->L);
	}
	bfree(data);
}

bool lua_filter_reload(obs_properties_t *props, obs_property_t *property, void *data)
{
	filter_data* filter = (filter_data*)data;

	if (filter->L)
	{
		lua_cb(filter, destroy_ref)
		{
			lua_ld_cb(filter->L, filter->source_info.destroy_ref);
			pushdata(filter);
			int res;
			lua_dpcall(filter->L, 1, 0, 0, res);
		}
		lua_close(filter->L);
		filter->L = NULL;
	}
	filter->source_info = { 0 };
	filter->data_ref = -1;

	return true;
}

obs_properties_t* filter_get_properties(void *data)
{
	filter_data* filter = (filter_data*)data;
	obs_properties_t *props = obs_properties_create();

	obs_properties_add_path(props, SETTING_LUA_FILE_PATH, TEXT_LUA_FILE_PATH, OBS_PATH_FILE, "Lua files (*.lua)", NULL);
	obs_properties_add_button(props, SETTING_LUA_RELOAD_BUTTON, TEXT_LUA_RELOAD_BUTTON, lua_filter_reload);

	lua_cb(filter, get_properties_ref)
	{
		lua_ld_cb(filter->L, filter->source_info.get_properties_ref);
		pushdata(filter);
		lua_push_cpointer(filter->L, props);
		int res;
		lua_dpcall(filter->L, 2, 0, 0, res);
	}

	return props;
}

void filter_video_render(void* data, gs_effect_t* effect)
{
	filter_data* filter = (filter_data*)data;

	lua_cb(filter, video_render_ref)
	{
		lua_ld_cb(filter->L, filter->source_info.video_render_ref);
		pushdata(filter);
		lua_push_cpointer(filter->L, effect);
		int res;
		lua_dpcall(filter->L, 2, 0, 0, res);
	}
}

uint32_t filter_get_width(void* data)
{
	filter_data* filter = (filter_data*)data;

	lua_cb(filter, get_width_ref)
	{
		lua_ld_cb(filter->L, filter->source_info.get_width_ref);
		pushdata(filter);
		int res;
		lua_dpcall(filter->L, 1, 1, 0, res);
		return lua_tointeger(filter->L, -1);
	}
	else
	{
		return 0;
	}
}

uint32_t filter_get_height(void* data)
{
	filter_data* filter = (filter_data*)data;

	lua_cb(filter, get_height_ref)
	{
		lua_ld_cb(filter->L, filter->source_info.get_height_ref);
		pushdata(filter);
		int res;
		lua_dpcall(filter->L, 1, 1, 0, res);
		return lua_tointeger(filter->L, -1);
	}
	else
	{
		return 0;
	}
}

void filter_video_tick(void* data, float seconds)
{
	filter_data* filter = (filter_data*)data;

	lua_cb(filter, video_tick_ref)
	{
		lua_ld_cb(filter->L, filter->source_info.video_tick_ref);
		pushdata(filter);
		lua_pushnumber(filter->L, seconds);
		int res;
		lua_dpcall(filter->L, 2, 0, 0, res);
	}
}

obs_source_info f;
obs_source_info* setup_lua_video_filter()
{
	f.id = "lua_video_filter";
	f.type = OBS_SOURCE_TYPE_FILTER;
	f.output_flags = OBS_SOURCE_VIDEO;
	f.get_name = filter_get_name;
	f.create = filter_create;
	f.destroy = filter_destroy;
	f.get_properties = filter_get_properties;
	f.update = filter_update;
	f.video_render = filter_video_render;
	f.get_width = filter_get_width;
	f.get_height = filter_get_height;
	f.video_tick = filter_video_tick;
	return &f;
}
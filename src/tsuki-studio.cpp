#include <string>
#include <Windows.h>

#include "obs-module.h"
#include "lua.hpp"

#include "lua_obs.h"
#include "lua_helper.h"
#include "lua-video-filter.h"

OBS_DECLARE_MODULE()
OBS_MODULE_USE_DEFAULT_LOCALE("tsuki-studio", "en-US")

#pragma region lua obs_module
int lua_obs_module_file(lua_State* L)
{
	int args;
	lua_failargs(L, 1, args);

	lua_pushstring(L, obs_module_file(lua_tostring(L, 1)));
	return 1;
}

int lua_obs_module_text(lua_State *L)
{
	int args;
	lua_failargs(L, 1, args);

	lua_pushstring(L, obs_module_text(lua_tostring(L, 1)));
	return 1;
}

const luaL_Reg modulelib[] = {
	{ "file", lua_obs_module_file },
	{ "text", lua_obs_module_text },
	{ NULL, NULL },
};

int luaopen_obsmodulelib(lua_State* L)
{
	luaL_newlib(L, modulelib);
	return 1;
}

void lua_open_obsmodule(lua_State* L)
{
	luaL_requiref(L, "obsmodule", luaopen_obsmodulelib, 1);
	lua_pop(L, 1);
}
#pragma endregion

int register_lua_video_filter(const char* lua_file)
{
	lua_State* L = newLstate();

	int result = luaL_loadfile(L, lua_file);
	bool failed = true;

	if (result == LUA_OK)
	{
		result = lua_pcall(L, 0, LUA_MULTRET, 0);

		if (result == LUA_OK)
		{
			int rets = lua_gettop(L);
			if (rets == 2 && lua_istable(L, 2))
			{

			}
		}
	}

	if (failed)
	{
		lua_print_error(L);
	}

	return result;
}

bool obs_module_load()
{
	/*char* mainlua_path = obs_module_file("main.lua");

	if (mainlua_path)
	{
		lua_State *state = newstate();

		int result = luaL_loadfile(state, mainlua_path);

		if (result != LUA_OK)
		{
			lua_print_error(state);
		}
		else
		{
			result = lua_pcall(state, 0, LUA_MULTRET, 0);

			if (result != LUA_OK)
			{
				lua_print_error(state);
			}
		}
	}

	char* video_filter_path = obs_module_file("video_filter");

	if (video_filter_path)
	{
		WIN32_FIND_DATA fd;
		std::string dir_path = video_filter_path;
		dir_path = dir_path.append("/*.lua");
		HANDLE h = ::FindFirstFile(dir_path.c_str(), &fd);
		if (h != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					std::string lua_file = "video_filter/";
					lua_file = lua_file.append(fd.cFileName);
					char* lua_file_cstr = obs_module_file(lua_file.c_str());
					register_lua_video_filter(lua_file_cstr);
				}
			}
			while (::FindNextFile(h, &fd));
			::FindClose(h);
		}
	}*/

	obs_register_source(setup_lua_video_filter());

	return true;
}
#pragma once
#include "lua.hpp"

void lua_open_helper(lua_State* L);
void lua_print_error(lua_State* L);
void lua_print_table(lua_State* L, int idx);
lua_State* newLstate();

#define errstr(msg) msg " @ " __FUNCTION__ " ! " __FILE__ ":" STRINGIFY_(__LINE__)
#define lua_throw(L, msg) lua_pushstring(L, errstr(msg)); return lua_error(L);
#define lua_failargs(L, ac, a) a = lua_gettop(L); if(a != ac) { lua_pushstring(L, errstr("wrong argument count")); return lua_error(L); }
#define lua_failtype(L, idx, t) if(lua_type(L, idx) != t) { lua_pushstring(L, errstr("wrong argument type")); return lua_error(L); }
#define lua_cfailtype(L, idx, t) assert(lua_type(L, idx) == t)
#define lua_ld_cb(L, ref) lua_rawgeti(L, LUA_REGISTRYINDEX, ref)
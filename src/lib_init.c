/*
** Library initialization.
** Copyright (C) 2005-2015 Mike Pall. See Copyright Notice in luajit.h
**
** Major parts taken verbatim from the Lua interpreter.
** Copyright (C) 1994-2008 Lua.org, PUC-Rio. See Copyright Notice in lua.h
*/

#define lib_init_c
#define LUA_LIB

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "lj_arch.h"

static const luaL_Reg lj_lib_load[] = {
  { "",			luaopen_base_hack },
  { LUA_LOADLIBNAME,	luaopen_package_hack },
  { LUA_TABLIBNAME,	luaopen_table_hack },
  { LUA_IOLIBNAME,	luaopen_io },
  { LUA_OSLIBNAME,	luaopen_os_hack },
  { LUA_STRLIBNAME,	luaopen_string_hack },
  { LUA_MATHLIBNAME,	luaopen_math_hack },
  { LUA_DBLIBNAME,	luaopen_debug_hack },
  { LUA_BITLIBNAME,	luaopen_bit_hack },
  { LUA_JITLIBNAME,	luaopen_jit_hack },
  { NULL,		NULL }
};

static const luaL_Reg lj_lib_preload[] = {
#if LJ_HASFFI
  { LUA_FFILIBNAME,	luaopen_ffi },
#endif
  { NULL,		NULL }
};

LUALIB_API void luaL_openlibs_hack(lua_State *L)
{
  const luaL_Reg *lib;
  for (lib = lj_lib_load; lib->func; lib++) {
    lua_pushcfunction(L, lib->func);
    lua_pushstring_hack(L, lib->name);
    lua_call_hack(L, 1, 0);
  }
  luaL_findtable_hack(L, LUA_REGISTRYINDEX, "_PRELOAD",
		 sizeof(lj_lib_preload)/sizeof(lj_lib_preload[0])-1);
  for (lib = lj_lib_preload; lib->func; lib++) {
    lua_pushcfunction(L, lib->func);
    lua_setfield_hack(L, -2, lib->name);
  }
  lua_pop(L, 1);
}


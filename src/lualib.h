/*
** Standard library header.
** Copyright (C) 2005-2015 Mike Pall. See Copyright Notice in luajit.h
*/

#ifndef _LUALIB_H
#define _LUALIB_H

#include "lua.h"

#define LUA_FILEHANDLE	"FILE*"

#define LUA_COLIBNAME	"coroutine"
#define LUA_MATHLIBNAME	"math"
#define LUA_STRLIBNAME	"string"
#define LUA_TABLIBNAME	"table"
#define LUA_IOLIBNAME	"io"
#define LUA_OSLIBNAME	"os"
#define LUA_LOADLIBNAME	"package"
#define LUA_DBLIBNAME	"debug"
#define LUA_BITLIBNAME	"bit"
#define LUA_JITLIBNAME	"jit"
#define LUA_FFILIBNAME	"ffi"

LUALIB_API int luaopen_base_hack(lua_State *L);
LUALIB_API int luaopen_math_hack(lua_State *L);
LUALIB_API int luaopen_string_hack(lua_State *L);
LUALIB_API int luaopen_table_hack(lua_State *L);
LUALIB_API int luaopen_io(lua_State *L);
LUALIB_API int luaopen_os_hack(lua_State *L);
LUALIB_API int luaopen_package_hack(lua_State *L);
LUALIB_API int luaopen_debug_hack(lua_State *L);
LUALIB_API int luaopen_bit_hack(lua_State *L);
LUALIB_API int luaopen_jit_hack(lua_State *L);
LUALIB_API int luaopen_ffi(lua_State *L);

LUALIB_API void luaL_openlibs_hack(lua_State *L);

#ifndef lua_assert
#define lua_assert(x)	((void)0)
#endif

#endif

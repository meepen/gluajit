
#ifdef LUA_BUILD_AS_DLL
#define WAS_BUILD_AS_DLL
#undef LUA_BUILD_AS_DLL
#endif


extern "C" {
#include "lua.h"
#include "luajit.h"
#include "lauxlib.h"
#include "lualib.h"
}

#ifdef WAS_BUILD_AS_DLL
#define LUA_BUILD_AS_DLL
#endif


#ifdef LUA_BUILD_AS_DLL
#include <Windows.h>

HMODULE RealLuaShared = 0;

#define IMPORT(x) \
void * real##x = 0; \
extern "C" __declspec(dllexport) __declspec(naked) void x() { \
    __asm { pushad } \
    if (!real##x) { \
        if (!RealLuaShared) \
            RealLuaShared = LoadLibraryA("lua_shared2.dll"); \
        real##x = GetProcAddress(RealLuaShared, #x); \
    } \
    __asm { popad } \
    __asm { jmp real##x } \
}

#define IMPORT_INJECT(original, new) \
const void * hackptr_##new = &new##_hack; \
extern "C" __declspec(dllexport) __declspec(naked) void original() { \
    __asm { jmp new##_hack } \
} \
struct strux##original { \
    strux##original() { \
        DWORD trash; \
        DWORD lpflOldProtect; \
        printf("%s\n", #original); \
        void * real = 0; \
        if (!real) { \
            if (!RealLuaShared) \
                RealLuaShared = LoadLibraryA("lua_shared2.dll"); \
            real = GetProcAddress(RealLuaShared, #original); \
            VirtualProtect(real, 7, PAGE_READWRITE, &lpflOldProtect); \
            *(unsigned char *)real = 0x90; \
            *(1 + (unsigned char *)real) = 0xFF; \
            *(2 + (unsigned char *)real) = 0x25; \
            *(void ***)(3 + (unsigned char *)real) = (void **)&hackptr_##new; \
            VirtualProtect(real, 7, lpflOldProtect, &trash); \
        } \
    } \
}; \
static strux##original construct_##original;

// FF 25 xx xx xx xx = jmp dword ptr [xxxxxxxx]
#else
#define IMPORT_INJECT(x, x)
#endif

IMPORT(CreateInterface)
IMPORT(GMOD_LoadBinaryModule)
IMPORT_INJECT(luaJIT_setmode, luaJIT_setmode)
IMPORT_INJECT(luaJIT_version_2_0_4, luaJIT_version_2_0_4)
IMPORT_INJECT(luaL_addlstring, luaL_addlstring)
IMPORT_INJECT(luaL_addstring, luaL_addstring)
IMPORT_INJECT(luaL_addvalue, luaL_addvalue)
IMPORT_INJECT(luaL_argerror, luaL_argerror)
IMPORT_INJECT(luaL_buffinit, luaL_buffinit)
IMPORT_INJECT(luaL_callmeta, luaL_callmeta)
IMPORT_INJECT(luaL_checkany, luaL_checkany)
IMPORT_INJECT(luaL_checkinteger, luaL_checkinteger)
IMPORT_INJECT(luaL_checklstring, luaL_checklstring)
IMPORT_INJECT(luaL_checknumber, luaL_checknumber)
IMPORT_INJECT(luaL_checkoption, luaL_checkoption)
IMPORT_INJECT(luaL_checkstack, luaL_checkstack)
IMPORT_INJECT(luaL_checktype, luaL_checktype)
IMPORT_INJECT(luaL_checkudata, luaL_checkudata)
IMPORT_INJECT(luaL_error, luaL_error)
IMPORT_INJECT(luaL_execresult, luaL_execresult)
IMPORT_INJECT(luaL_fileresult, luaL_fileresult)
IMPORT_INJECT(luaL_findtable, luaL_findtable)
IMPORT_INJECT(luaL_getmetafield, luaL_getmetafield)
IMPORT_INJECT(luaL_gsub, luaL_gsub)
IMPORT_INJECT(luaL_loadbuffer, luaL_loadbuffer)
IMPORT_INJECT(luaL_loadbufferx, luaL_loadbufferx)
IMPORT_INJECT(luaL_loadfile, luaL_loadfile)
IMPORT_INJECT(luaL_loadfilex, luaL_loadfilex)
IMPORT_INJECT(luaL_loadstring, luaL_loadstring)
IMPORT_INJECT(luaL_newmetatable, luaL_newmetatable)
IMPORT(luaL_newmetatable_type)
IMPORT_INJECT(luaL_newstate, luaL_newstate)
IMPORT_INJECT(luaL_openlib, luaL_openlib)
IMPORT_INJECT(luaL_openlibs, luaL_openlibs)
IMPORT_INJECT(luaL_optinteger, luaL_optinteger)
IMPORT_INJECT(luaL_optlstring, luaL_optlstring)
IMPORT_INJECT(luaL_optnumber, luaL_optnumber)
IMPORT_INJECT(luaL_prepbuffer, luaL_prepbuffer)
IMPORT_INJECT(luaL_pushresult, luaL_pushresult)
IMPORT_INJECT(luaL_ref, luaL_ref)
IMPORT_INJECT(luaL_register, luaL_register)
IMPORT_INJECT(luaL_traceback, luaL_traceback)
IMPORT_INJECT(luaL_typerror, luaL_typerror)
IMPORT_INJECT(luaL_unref, luaL_unref)
IMPORT_INJECT(luaL_where, luaL_where)
IMPORT_INJECT(lua_atpanic, lua_atpanic)
IMPORT_INJECT(lua_call, lua_call)
IMPORT_INJECT(lua_checkstack, lua_checkstack)
IMPORT_INJECT(lua_close, lua_close)
IMPORT_INJECT(lua_concat, lua_concat)
IMPORT_INJECT(lua_cpcall, lua_cpcall)
IMPORT_INJECT(lua_createtable, lua_createtable)
IMPORT_INJECT(lua_dump, lua_dump)
IMPORT_INJECT(lua_equal, lua_equal)
IMPORT_INJECT(lua_error, lua_error)
IMPORT_INJECT(lua_gc, lua_gc)
IMPORT_INJECT(lua_getallocf, lua_getallocf)
IMPORT_INJECT(lua_getfenv, lua_getfenv)
IMPORT_INJECT(lua_getfield, lua_getfield)
IMPORT_INJECT(lua_gethook, lua_gethook)
IMPORT_INJECT(lua_gethookcount, lua_gethookcount)
IMPORT_INJECT(lua_gethookmask, lua_gethookmask)
IMPORT_INJECT(lua_getinfo, lua_getinfo)
IMPORT_INJECT(lua_getlocal, lua_getlocal)
IMPORT_INJECT(lua_getmetatable, lua_getmetatable)
IMPORT_INJECT(lua_getstack, lua_getstack)
IMPORT_INJECT(lua_gettable, lua_gettable)
IMPORT_INJECT(lua_gettop, lua_gettop)
IMPORT_INJECT(lua_getupvalue, lua_getupvalue)
IMPORT_INJECT(lua_insert, lua_insert)
IMPORT_INJECT(lua_iscfunction, lua_iscfunction)
IMPORT_INJECT(lua_isnumber, lua_isnumber)
IMPORT_INJECT(lua_isstring, lua_isstring)
IMPORT_INJECT(lua_isuserdata, lua_isuserdata)
IMPORT_INJECT(lua_lessthan, lua_lessthan)
IMPORT_INJECT(lua_load, lua_load)
IMPORT_INJECT(lua_loadx, lua_loadx)
IMPORT_INJECT(lua_newstate, lua_newstate)
IMPORT_INJECT(lua_newthread, lua_newthread)
IMPORT_INJECT(lua_newuserdata, lua_newuserdata)
IMPORT_INJECT(lua_next, lua_next)
IMPORT_INJECT(lua_objlen, lua_objlen)
IMPORT_INJECT(lua_pcall, lua_pcall)
IMPORT_INJECT(lua_pushboolean, lua_pushboolean)
IMPORT_INJECT(lua_pushcclosure, lua_pushcclosure)
IMPORT_INJECT(lua_pushfstring, lua_pushfstring)
IMPORT_INJECT(lua_pushinteger, lua_pushinteger)
IMPORT_INJECT(lua_pushlightuserdata, lua_pushlightuserdata)
IMPORT_INJECT(lua_pushlstring, lua_pushlstring)
IMPORT_INJECT(lua_pushnil, lua_pushnil)
IMPORT_INJECT(lua_pushnumber, lua_pushnumber)
IMPORT_INJECT(lua_pushstring, lua_pushstring)
IMPORT_INJECT(lua_pushthread, lua_pushthread)
IMPORT_INJECT(lua_pushvalue, lua_pushvalue)
IMPORT_INJECT(lua_pushvfstring, lua_pushvfstring)
IMPORT_INJECT(lua_rawequal, lua_rawequal)
IMPORT_INJECT(lua_rawget, lua_rawget)
IMPORT_INJECT(lua_rawgeti, lua_rawgeti)
IMPORT_INJECT(lua_rawset, lua_rawset)
IMPORT_INJECT(lua_rawseti, lua_rawseti)
IMPORT_INJECT(lua_remove, lua_remove)
IMPORT_INJECT(lua_replace, lua_replace)
IMPORT_INJECT(lua_resume_real, lua_resume)
IMPORT_INJECT(lua_setallocf, lua_setallocf)
IMPORT_INJECT(lua_setfenv, lua_setfenv)
IMPORT_INJECT(lua_setfield, lua_setfield)
IMPORT_INJECT(lua_sethook, lua_sethook)
IMPORT_INJECT(lua_setlocal, lua_setlocal)
IMPORT_INJECT(lua_setmetatable, lua_setmetatable)
IMPORT_INJECT(lua_settable, lua_settable)
IMPORT_INJECT(lua_settop, lua_settop)
IMPORT_INJECT(lua_setupvalue, lua_setupvalue)
IMPORT_INJECT(lua_status, lua_status)
IMPORT_INJECT(lua_toboolean, lua_toboolean)
IMPORT_INJECT(lua_tocfunction, lua_tocfunction)
IMPORT_INJECT(lua_tointeger, lua_tointeger)
IMPORT_INJECT(lua_tolstring, lua_tolstring)
IMPORT_INJECT(lua_tonumber, lua_tonumber)
IMPORT_INJECT(lua_topointer, lua_topointer)
IMPORT_INJECT(lua_tothread, lua_tothread)
IMPORT_INJECT(lua_touserdata, lua_touserdata)
IMPORT_INJECT(lua_type, lua_type)
IMPORT_INJECT(lua_typename, lua_typename)
IMPORT_INJECT(lua_upvalueid, lua_upvalueid)
IMPORT_INJECT(lua_upvaluejoin, lua_upvaluejoin)
IMPORT_INJECT(lua_xmove, lua_xmove)
IMPORT_INJECT(lua_yield, lua_yield)
IMPORT_INJECT(luaopen_base, luaopen_base)
IMPORT_INJECT(luaopen_bit, luaopen_bit)
IMPORT_INJECT(luaopen_debug, luaopen_debug)
IMPORT_INJECT(luaopen_jit, luaopen_jit)
IMPORT_INJECT(luaopen_math, luaopen_math)
IMPORT_INJECT(luaopen_os, luaopen_os)
IMPORT_INJECT(luaopen_package, luaopen_package)
IMPORT_INJECT(luaopen_string, luaopen_string)
IMPORT_INJECT(luaopen_table, luaopen_table)
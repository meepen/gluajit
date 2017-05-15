#ifdef LUA_BUILD_AS_DLL
    #define WAS_LUA_BUILD_AS_DLL
    #undef LUA_BUILD_AS_DLL
#endif

extern "C" {
    #include "lua.h"
    #include "luajit.h"
    #include "lauxlib.h"
    #include "lualib.h"
    #include "lj_arch.h"
    #include "luaconf.h"
}

// Use here to include things automatically so we don't need to change the makefiles and build scripts
#include "management.cpp"

#ifdef WAS_LUA_BUILD_AS_DLL
    #undef WAS_LUA_BUILD_AS_DLL
    #define LUA_BUILD_AS_DLL
#endif

static void *ll_load(const char *path, int gl = 0);
static void *ll_sym(void *lib, const char *sym);
enum EProtection;
static void ll_mprotect(void *addr, size_t size, EProtection prot);

#if LJ_TARGET_WINDOWS
    #include <Windows.h>
    enum EProtection {
        PROTECTION_READWRITE = PAGE_READWRITE,
        PROTECTION_READEXEC  = PAGE_EXECUTE_READ
    };
    static void ll_mprotect(void *addr, size_t size, EProtection prot)
    {
        DWORD lpflOldProtect;
        VirtualProtect(addr, size, prot, &lpflOldProtect);
    }
#elif LJ_TARGET_POSIX
    #include <sys/mman.h>
    enum EProtection {
        PROTECTION_READWRITE = PROT_READ | PROT_WRITE,
        PROTECTION_READEXEC  = PROT_READ | PROT_EXEC
    };
    static void ll_mprotect(void *addr, size_t size, EProtection prot)
    {
        mprotect(addr, size, (int)prot);
    }
#endif

#if LJ_TARGET_DLOPEN
    #include <dlfcn.h>
    static void *ll_load(const char *path, int gl)
    {
      void *lib = dlopen(path, RTLD_NOW | (gl ? RTLD_GLOBAL : RTLD_LOCAL));
      return lib;
    }

    static void *ll_sym(void *lib, const char *sym)
    {
      void *f = (void *)dlsym(lib, sym);
      return f;
    }
#elif LJ_TARGET_WINDOWS
    static void *ll_load(const char *path, int gl)
    {
      void *lib = (void *)LoadLibraryA(path);
      return lib;
    }

    static void *ll_sym(void *lib, const char *sym)
    {
      void *f = (void *)GetProcAddress((HINSTANCE)lib, sym);
      return f;
    }
#else
    #error "what os is this?"
#endif

void *RealLuaShared = 0;
void *RealDetoured  = 0;

#define MODULE_NAME "garrysmod/bin/lua_shared.dll"

#define REDIRECT(x) \
void * real##x = 0; \
extern "C" __declspec(dllexport) __declspec(naked) void x() { \
    __asm { pushad } \
    if (!real##x) { \
        if (!RealDetoured) \
            RealDetoured = ll_load("real_datacache.dll", 0); \
        real##x = ll_sym(RealDetoured, #x); \
    } \
    __asm { popad } \
    __asm { jmp real##x } \
}

#define IMPORT_INJECT(original) \
const void * hackptr_##original = &original; \
struct strux##original { \
    strux##original() { \
        printf("%s\n", #original); \
        void *real = 0; \
        if (!real) { \
            if (!RealLuaShared) \
                RealLuaShared = ll_load(MODULE_NAME); \
            real = ll_sym(RealLuaShared, #original); \
            ll_mprotect(real, 7, PROTECTION_READWRITE); \
            *(unsigned char *)real = 0x90; \
            *(1 + (unsigned char *)real) = 0xFF; \
            *(2 + (unsigned char *)real) = 0x25; \
            *(void ***)(3 + (unsigned char *)real) = (void **)&hackptr_##original; \
            ll_mprotect(real, 7, PROTECTION_READEXEC); \
        } \
    } \
}; \
static strux##original construct_##original;

#define INITIALIZER(name, def) \
struct init##name { \
    init##name() def \
}; \
static init##name initialier_##name;

const int libopen_base_offset1 = 0x61, libopen_base_offset2 = 0x66;

extern "C" void *lj_lib_cf_gmod_base = 0, *lj_lib_init_gmod_base = 0;
#if LJ_TARGET_WINDOWS
    INITIALIZER(libopen_base, {
        if (!RealLuaShared)
            RealLuaShared = ll_load(MODULE_NAME);
        void *real_lob = ll_sym(RealLuaShared, "luaopen_base");

        char *push_addresses = libopen_base_offset1 + (char *)real_lob;
        // assert(push_addresses[0] == 0x68) // push offset
        lj_lib_cf_gmod_base = *(void **)&push_addresses[1];

        push_addresses = libopen_base_offset2 + (char *)real_lob;
        // assert(push_addresses[0] == 0x68) // push offset
        lj_lib_init_gmod_base = *(void **)&push_addresses[1];
    })
    // FF 25 xx xx xx xx = jmp dword ptr [xxxxxxxx]
#elif LJ_TARGET_LINUX
    INITIALIZER(libopen_base, {
        if (!RealLuaShared)
            RealLuaShared = ll_load(MODULE_NAME);
        void *real_lob = ll_sym(RealLuaShared, "luaopen_base");

        char *push_addresses = libopen_base_offset1 + (char *)real_lob;
        // assert(push_addresses[0] == 0x68) // push offset
        lj_lib_cf_gmod_base = *(void **)&push_addresses[1];

        push_addresses = libopen_base_offset2 + (char *)real_lob;
        // assert(push_addresses[0] == 0x68) // push offset
        lj_lib_init_gmod_base = *(void **)&push_addresses[1];
    })
#else
    #error "need to do this os"
#endif

REDIRECT(CreateInterface)
IMPORT_INJECT(luaJIT_setmode)
IMPORT_INJECT(luaJIT_version_2_0_4)
IMPORT_INJECT(luaL_addlstring)
IMPORT_INJECT(luaL_addstring)
IMPORT_INJECT(luaL_addvalue)
IMPORT_INJECT(luaL_argerror)
IMPORT_INJECT(luaL_buffinit)
IMPORT_INJECT(luaL_callmeta)
IMPORT_INJECT(luaL_checkany)
IMPORT_INJECT(luaL_checkinteger)
IMPORT_INJECT(luaL_checklstring)
IMPORT_INJECT(luaL_checknumber)
IMPORT_INJECT(luaL_checkoption)
IMPORT_INJECT(luaL_checkstack)
IMPORT_INJECT(luaL_checktype)
IMPORT_INJECT(luaL_checkudata)
IMPORT_INJECT(luaL_error)
IMPORT_INJECT(luaL_execresult)
IMPORT_INJECT(luaL_fileresult)
IMPORT_INJECT(luaL_findtable)
IMPORT_INJECT(luaL_getmetafield)
IMPORT_INJECT(luaL_gsub)
IMPORT_INJECT(luaL_loadbuffer)
IMPORT_INJECT(luaL_loadbufferx)
IMPORT_INJECT(luaL_loadfile)
IMPORT_INJECT(luaL_loadfilex)
IMPORT_INJECT(luaL_loadstring)
IMPORT_INJECT(luaL_newmetatable)
IMPORT_INJECT(luaL_newmetatable_type)
IMPORT_INJECT(luaL_newstate)
IMPORT_INJECT(luaL_openlib)
IMPORT_INJECT(luaL_openlibs)
IMPORT_INJECT(luaL_optinteger)
IMPORT_INJECT(luaL_optlstring)
IMPORT_INJECT(luaL_optnumber)
IMPORT_INJECT(luaL_prepbuffer)
IMPORT_INJECT(luaL_pushresult)
IMPORT_INJECT(luaL_ref)
IMPORT_INJECT(luaL_register)
IMPORT_INJECT(luaL_traceback)
IMPORT_INJECT(luaL_typerror)
IMPORT_INJECT(luaL_unref)
IMPORT_INJECT(luaL_where)
IMPORT_INJECT(lua_atpanic)
IMPORT_INJECT(lua_call)
IMPORT_INJECT(lua_checkstack)
IMPORT_INJECT(lua_close)
IMPORT_INJECT(lua_concat)
IMPORT_INJECT(lua_cpcall)
IMPORT_INJECT(lua_createtable)
IMPORT_INJECT(lua_dump)
IMPORT_INJECT(lua_equal)
IMPORT_INJECT(lua_error)
IMPORT_INJECT(lua_gc)
IMPORT_INJECT(lua_getallocf)
IMPORT_INJECT(lua_getfenv)
IMPORT_INJECT(lua_getfield)
IMPORT_INJECT(lua_gethook)
IMPORT_INJECT(lua_gethookcount)
IMPORT_INJECT(lua_gethookmask)
IMPORT_INJECT(lua_getinfo)
IMPORT_INJECT(lua_getlocal)
IMPORT_INJECT(lua_getmetatable)
IMPORT_INJECT(lua_getstack)
IMPORT_INJECT(lua_gettable)
IMPORT_INJECT(lua_gettop)
IMPORT_INJECT(lua_getupvalue)
IMPORT_INJECT(lua_insert)
IMPORT_INJECT(lua_iscfunction)
IMPORT_INJECT(lua_isnumber)
IMPORT_INJECT(lua_isstring)
IMPORT_INJECT(lua_isuserdata)
IMPORT_INJECT(lua_lessthan)
IMPORT_INJECT(lua_load)
IMPORT_INJECT(lua_loadx)
IMPORT_INJECT(lua_newstate)
IMPORT_INJECT(lua_newthread)
IMPORT_INJECT(lua_newuserdata)
IMPORT_INJECT(lua_next)
IMPORT_INJECT(lua_objlen)
IMPORT_INJECT(lua_pcall)
IMPORT_INJECT(lua_pushboolean)
IMPORT_INJECT(lua_pushcclosure)
IMPORT_INJECT(lua_pushfstring)
IMPORT_INJECT(lua_pushinteger)
IMPORT_INJECT(lua_pushlightuserdata)
IMPORT_INJECT(lua_pushlstring)
IMPORT_INJECT(lua_pushnil)
IMPORT_INJECT(lua_pushnumber)
IMPORT_INJECT(lua_pushstring)
IMPORT_INJECT(lua_pushthread)
IMPORT_INJECT(lua_pushvalue)
IMPORT_INJECT(lua_pushvfstring)
IMPORT_INJECT(lua_rawequal)
IMPORT_INJECT(lua_rawget)
IMPORT_INJECT(lua_rawgeti)
IMPORT_INJECT(lua_rawset)
IMPORT_INJECT(lua_rawseti)
IMPORT_INJECT(lua_remove)
IMPORT_INJECT(lua_replace)
IMPORT_INJECT(lua_resume_real)
IMPORT_INJECT(lua_setallocf)
IMPORT_INJECT(lua_setfenv)
IMPORT_INJECT(lua_setfield)
IMPORT_INJECT(lua_sethook)
IMPORT_INJECT(lua_setlocal)
IMPORT_INJECT(lua_setmetatable)
IMPORT_INJECT(lua_settable)
IMPORT_INJECT(lua_settop)
IMPORT_INJECT(lua_setupvalue)
IMPORT_INJECT(lua_status)
IMPORT_INJECT(lua_toboolean)
IMPORT_INJECT(lua_tocfunction)
IMPORT_INJECT(lua_tointeger)
IMPORT_INJECT(lua_tolstring)
IMPORT_INJECT(lua_tonumber)
IMPORT_INJECT(lua_topointer)
IMPORT_INJECT(lua_tothread)
IMPORT_INJECT(lua_touserdata)
IMPORT_INJECT(lua_type)
IMPORT_INJECT(lua_typename)
IMPORT_INJECT(lua_upvalueid)
IMPORT_INJECT(lua_upvaluejoin)
IMPORT_INJECT(lua_xmove)
IMPORT_INJECT(lua_yield)
IMPORT_INJECT(luaopen_base)
IMPORT_INJECT(luaopen_bit)
IMPORT_INJECT(luaopen_debug)
IMPORT_INJECT(luaopen_jit)
IMPORT_INJECT(luaopen_math)
IMPORT_INJECT(luaopen_os)
IMPORT_INJECT(luaopen_package)
IMPORT_INJECT(luaopen_string)
IMPORT_INJECT(luaopen_table)

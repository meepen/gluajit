// We already have lua and friends included - don't include stuff
extern "C" {
#include "management.h"
#include "setjmp.h"
}

#include <stdio.h>

#include <mutex>

static std::mutex m;
static jmp_buf buf;
template <typename T = void>
static bool lua_safe(void (*body)(T *arg), T *arg = 0)
{
    m.lock();
    bool success = true;
    if (!setjmp(buf))
        body(arg);
    else
        success = false;
    
    m.unlock();
    return success;
}
static int panicfn(lua_State *state) {
    FILE *f = fopen("err.txt", "wb");
    luaL_traceback(state, state, lua_tostring(state, -1), 0);
    fprintf(f, "%s\n\n", lua_tostring(state, -1));
    fclose(f);
    lua_pop(state, 2);
    longjmp(buf, 1);
    return 0;
}

static void push_hook_call(lua_State *L)
{
    lua_getfield(L, LUA_GLOBALSINDEX, "hook"); // +1
    lua_getfield(L, -1, "Call");               // +2
    lua_remove(L, -2);
}

lua_State *MainState;
bool InManagement = false;

extern "C" void hijack_CreateManagementState(void) {
    InManagement = true;
    MainState = luaL_newstate();
    lua_atpanic(MainState, panicfn);
    luaL_openlibs(MainState);
    {
        // package.path = "./management/?.lua"..package.path
        lua_getfield(MainState, LUA_GLOBALSINDEX, "package");
        lua_pushstring(MainState, "." LUA_DIRSEP "management" LUA_DIRSEP "?.lua;");
        lua_getfield(MainState, -2, "path");
        lua_concat(MainState, 2);
        lua_setfield(MainState, -2, "path");
        lua_pop(MainState, 1);
    }
    if (!lua_safe<void>([](void *) {
        if (0 == luaL_loadfile(MainState, "management/init.lua"))
            lua_call(MainState, 0, 0);
    }))
        exit(1);
        
    InManagement = false;
}

extern "C" void hijack_StateCreation(lua_State *state) {

}

extern "C" int hijack_LibOpen(lua_State *state, LibOpenType type) {
    if (InManagement)
        return type != LIBTYPE_GMODBASE;

    struct Arg {
        int ShouldOpen;
        LibOpenType type;
        lua_State *state;
    } arg;
    arg.ShouldOpen = 0;
    arg.type = type;
    arg.state = state;

    lua_safe<Arg>([](Arg *arg) {
        push_hook_call(MainState);
        lua_pushstring(MainState, "LibOpen");
        lua_pushinteger(MainState, *(lua_Integer *)&arg->state);
        lua_pushstring(MainState, libtype_strings[arg->type]);
        lua_call(MainState, 3, 1);
        arg->ShouldOpen = lua_toboolean(MainState, -1) ? 1 : 0;
        lua_pop(MainState, 1);
    }, &arg);

    return arg.ShouldOpen;
}
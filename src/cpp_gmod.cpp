extern "C" {
#include "lj_obj.h"
}

namespace GarrysMod {
    namespace Lua {
        class ILuaInterface;
    }
}

GarrysMod::Lua::ILuaInterface *GetInterface(lua_State *L) {
    return (GarrysMod::Lua::ILuaInterface *)L->luabase;
}
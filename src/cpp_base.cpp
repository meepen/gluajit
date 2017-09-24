#include "gmod/luainterface.h"

extern "C" void hijack_Print(lua_State *state, const char *str) {
    GetInterface(state)->GetCallback()->Msg(str, false);
}
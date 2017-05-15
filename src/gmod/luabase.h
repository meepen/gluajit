
#ifndef LUABASE_H
#define LUABASE_H

struct lua_State;

namespace GarrysMod
{
    namespace Lua 
    {
        class ILuaBase
        {
            virtual void fuck() = 0;
        public:
            void SetState(lua_State *state)
            {
                this->state = state;
            }
            lua_State *state;
        };
    };
};

#endif // LUABASE_H

#ifndef LUAINTERFACE_H
#define LUAINTERFACE_H

#include "luabase.h"
namespace std {
    class string;
}

struct lua_State;
struct Color;


namespace GarrysMod
{
    namespace Lua 
    {
        class ILuaObject;
        class ILuaInterface;

        class ILuaCallback
		{
		public:
			virtual ILuaObject *CreateLuaObject( ) = 0;
			virtual void DestroyLuaObject( ILuaObject *pObject ) = 0;

			virtual void ErrorPrint( const char *error, bool print ) = 0;

			virtual void Msg( const char *msg, bool useless ) = 0;
			virtual void MsgColour( const char *msg, const Color &color ) = 0;

			virtual void LuaError( const std::string &error ) = 0;

			virtual void InterfaceCreated( ILuaInterface *iface ) = 0;
		};
        class ILuaInterface : public ILuaBase
        {
            public:
            ILuaCallback *GetCallback(void) {
                return *(ILuaCallback **)(0xBC + (char *)this);
            }
        };
    };
};

GarrysMod::Lua::ILuaInterface *GetInterface(lua_State *L);

#endif // LUAINTERFACE_H
class IGet;
class CSteamAPIContext;
class ILuaInterface;
namespace GarrysMod {
namespace Lua {
class ILuaShared
{
public:
    virtual ~ILuaShared( ) = 0;
    virtual void Init( void *( * )( const char *, int * ), bool, CSteamAPIContext *, IGet * ) = 0;
    virtual void Shutdown( ) = 0;
    virtual void DumpStats( ) = 0;
    virtual ILuaInterface *CreateLuaInterface( unsigned char, bool ) = 0;
    virtual void CloseLuaInterface( ILuaInterface * ) = 0;
};
};
};
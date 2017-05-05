README for LuaJIT 2.0.4
-----------------------

LuaJIT is a Just-In-Time (JIT) compiler for the Lua programming language.

Project Homepage: http://luajit.org/

LuaJIT is Copyright (C) 2005-2015 Mike Pall.
LuaJIT is free software, released under the MIT license.
See full Copyright Notice in the COPYRIGHT file or in luajit.h.

Documentation for LuaJIT is available in HTML format.
Please point your favorite browser to:

 doc/luajit.html



README for gluajit 2.0.4
------------------------

To install on windows:

1. Move lua_shared.dll to hl2.exe's folder
2. rename lua_shared.dll to lua_shared2.dll
3. Compile with msvcbuild in a visual studio command prompt
4. Place lua51.dll in the old lua_shared.dll position with the same name
5. Launch Garry's Mod


THIS IS NOT SAFE FOR GENERAL USE ON SERVERS
-------------------------------------------

FFI is enabled and can be loaded by running 
```lua
ffi = package.loaders[1] "ffi"()
```
allowing any server to control your computer as they see fit. I recommend using this on servers only.

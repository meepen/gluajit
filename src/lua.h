/*
** $Id: lua.h,v 1.218.1.5 2008/08/06 13:30:12 roberto Exp $
** Lua - An Extensible Extension Language
** Lua.org, PUC-Rio, Brazil (http://www.lua.org)
** See Copyright Notice at the end of this file
*/


#ifndef lua_h
#define lua_h

#include <stdarg.h>
#include <stddef.h>


#include "luaconf.h"


#define LUA_VERSION	"Lua 5.1"
#define LUA_RELEASE	"Lua 5.1.4"
#define LUA_VERSION_NUM	501
#define LUA_COPYRIGHT	"Copyright (C) 1994-2008 Lua.org, PUC-Rio"
#define LUA_AUTHORS	"R. Ierusalimschy, L. H. de Figueiredo & W. Celes"


/* mark for precompiled code (`<esc>Lua') */
#define	LUA_SIGNATURE	"\033Lua"

/* option for multiple returns in `lua_pcall_hack' and `lua_call_hack' */
#define LUA_MULTRET	(-1)


/*
** pseudo-indices
*/
#define LUA_REGISTRYINDEX	(-10000)
#define LUA_ENVIRONINDEX	(-10001)
#define LUA_GLOBALSINDEX	(-10002)
#define lua_upvalueindex(i)	(LUA_GLOBALSINDEX-(i))


/* thread status; 0 is OK */
#define LUA_YIELD	1
#define LUA_ERRRUN	2
#define LUA_ERRSYNTAX	3
#define LUA_ERRMEM	4
#define LUA_ERRERR	5


typedef struct lua_State lua_State;

typedef int (*lua_CFunction) (lua_State *L);


/*
** functions that read/write blocks when loading/dumping Lua chunks
*/
typedef const char * (*lua_Reader) (lua_State *L, void *ud, size_t *sz);

typedef int (*lua_Writer) (lua_State *L, const void* p, size_t sz, void* ud);


/*
** prototype for memory-allocation functions
*/
typedef void * (*lua_Alloc) (void *ud, void *ptr, size_t osize, size_t nsize);


/*
** basic types
*/
#define LUA_TNONE		(-1)

#define LUA_TNIL		0
#define LUA_TBOOLEAN		1
#define LUA_TLIGHTUSERDATA	2
#define LUA_TNUMBER		3
#define LUA_TSTRING		4
#define LUA_TTABLE		5
#define LUA_TFUNCTION		6
#define LUA_TUSERDATA		7
#define LUA_TTHREAD		8



/* minimum Lua stack available to a C function */
#define LUA_MINSTACK	20


/*
** generic extra include file
*/
#if defined(LUA_USER_H)
#include LUA_USER_H
#endif


/* type of numbers in Lua */
typedef LUA_NUMBER lua_Number;


/* type for integer functions */
typedef LUA_INTEGER lua_Integer;



/*
** state manipulation
*/
LUA_API lua_State *(lua_newstate_hack) (lua_Alloc f, void *ud);
LUA_API void       (lua_close_hack) (lua_State *L);
LUA_API lua_State *(lua_newthread_hack) (lua_State *L);

LUA_API lua_CFunction (lua_atpanic_hack) (lua_State *L, lua_CFunction panicf);


/*
** basic stack manipulation
*/
LUA_API int   (lua_gettop_hack) (lua_State *L);
LUA_API void  (lua_settop_hack) (lua_State *L, int idx);
LUA_API void  (lua_pushvalue_hack) (lua_State *L, int idx);
LUA_API void  (lua_remove_hack) (lua_State *L, int idx);
LUA_API void  (lua_insert_hack) (lua_State *L, int idx);
LUA_API void  (lua_replace_hack) (lua_State *L, int idx);
LUA_API int   (lua_checkstack_hack) (lua_State *L, int sz);

LUA_API void  (lua_xmove_hack) (lua_State *from, lua_State *to, int n);


/*
** access functions (stack -> C)
*/

LUA_API int             (lua_isnumber_hack) (lua_State *L, int idx);
LUA_API int             (lua_isstring_hack) (lua_State *L, int idx);
LUA_API int             (lua_iscfunction_hack) (lua_State *L, int idx);
LUA_API int             (lua_isuserdata_hack) (lua_State *L, int idx);
LUA_API int             (lua_type_hack) (lua_State *L, int idx);
LUA_API const char     *(lua_typename_hack) (lua_State *L, int tp);

LUA_API int            (lua_equal_hack) (lua_State *L, int idx1, int idx2);
LUA_API int            (lua_rawequal_hack) (lua_State *L, int idx1, int idx2);
LUA_API int            (lua_lessthan_hack) (lua_State *L, int idx1, int idx2);

LUA_API lua_Number      (lua_tonumber_hack) (lua_State *L, int idx);
LUA_API lua_Integer     (lua_tointeger_hack) (lua_State *L, int idx);
LUA_API int             (lua_toboolean_hack) (lua_State *L, int idx);
LUA_API const char     *(lua_tolstring_hack) (lua_State *L, int idx, size_t *len);
LUA_API size_t          (lua_objlen_hack) (lua_State *L, int idx);
LUA_API lua_CFunction   (lua_tocfunction_hack) (lua_State *L, int idx);
LUA_API void	       *(lua_touserdata_hack) (lua_State *L, int idx);
LUA_API lua_State      *(lua_tothread_hack) (lua_State *L, int idx);
LUA_API const void     *(lua_topointer_hack) (lua_State *L, int idx);


/*
** push functions (C -> stack)
*/
LUA_API void  (lua_pushnil_hack) (lua_State *L);
LUA_API void  (lua_pushnumber_hack) (lua_State *L, lua_Number n);
LUA_API void  (lua_pushinteger_hack) (lua_State *L, lua_Integer n);
LUA_API void  (lua_pushlstring_hack) (lua_State *L, const char *s, size_t l);
LUA_API void  (lua_pushstring_hack) (lua_State *L, const char *s);
LUA_API const char *(lua_pushvfstring_hack) (lua_State *L, const char *fmt,
                                                      va_list argp);
LUA_API const char *(lua_pushfstring_hack) (lua_State *L, const char *fmt, ...);
LUA_API void  (lua_pushcclosure_hack) (lua_State *L, lua_CFunction fn, int n);
LUA_API void  (lua_pushboolean_hack) (lua_State *L, int b);
LUA_API void  (lua_pushlightuserdata_hack) (lua_State *L, void *p);
LUA_API int   (lua_pushthread_hack) (lua_State *L);


/*
** get functions (Lua -> stack)
*/
LUA_API void  (lua_gettable_hack) (lua_State *L, int idx);
LUA_API void  (lua_getfield_hack) (lua_State *L, int idx, const char *k);
LUA_API void  (lua_rawget_hack) (lua_State *L, int idx);
LUA_API void  (lua_rawgeti_hack) (lua_State *L, int idx, int n);
LUA_API void  (lua_createtable_hack) (lua_State *L, int narr, int nrec);
LUA_API void *(lua_newuserdata_hack) (lua_State *L, size_t sz);
LUA_API int   (lua_getmetatable_hack) (lua_State *L, int objindex);
LUA_API void  (lua_getfenv_hack) (lua_State *L, int idx);


/*
** set functions (stack -> Lua)
*/
LUA_API void  (lua_settable_hack) (lua_State *L, int idx);
LUA_API void  (lua_setfield_hack) (lua_State *L, int idx, const char *k);
LUA_API void  (lua_rawset_hack) (lua_State *L, int idx);
LUA_API void  (lua_rawseti_hack) (lua_State *L, int idx, int n);
LUA_API int   (lua_setmetatable_hack) (lua_State *L, int objindex);
LUA_API int   (lua_setfenv_hack) (lua_State *L, int idx);


/*
** `load' and `call' functions (load and run Lua code)
*/
LUA_API void  (lua_call_hack) (lua_State *L, int nargs, int nresults);
LUA_API int   (lua_pcall_hack) (lua_State *L, int nargs, int nresults, int errfunc);
LUA_API int   (lua_cpcall_hack) (lua_State *L, lua_CFunction func, void *ud);
LUA_API int   (lua_load_hack) (lua_State *L, lua_Reader reader, void *dt,
                                        const char *chunkname);

LUA_API int (lua_dump_hack) (lua_State *L, lua_Writer writer, void *data);


/*
** coroutine functions
*/
LUA_API int  (lua_yield_hack) (lua_State *L, int nresults);
LUA_API int  (lua_resume_hack) (lua_State *L, int narg);
LUA_API int  (lua_status_hack) (lua_State *L);

/*
** garbage-collection function and options
*/

#define LUA_GCSTOP		0
#define LUA_GCRESTART		1
#define LUA_GCCOLLECT		2
#define LUA_GCCOUNT		3
#define LUA_GCCOUNTB		4
#define LUA_GCSTEP		5
#define LUA_GCSETPAUSE		6
#define LUA_GCSETSTEPMUL	7

LUA_API int (lua_gc_hack) (lua_State *L, int what, int data);


/*
** miscellaneous functions
*/

LUA_API int   (lua_error_hack) (lua_State *L);

LUA_API int   (lua_next_hack) (lua_State *L, int idx);

LUA_API void  (lua_concat_hack) (lua_State *L, int n);

LUA_API lua_Alloc (lua_getallocf_hack) (lua_State *L, void **ud);
LUA_API void lua_setallocf_hack (lua_State *L, lua_Alloc f, void *ud);



/*
** ===============================================================
** some useful macros
** ===============================================================
*/

#define lua_pop(L,n)		lua_settop_hack(L, -(n)-1)

#define lua_newtable(L)		lua_createtable_hack(L, 0, 0)

#define lua_register(L,n,f) (lua_pushcfunction(L, (f)), lua_setglobal(L, (n)))

#define lua_pushcfunction(L,f)	lua_pushcclosure_hack(L, (f), 0)

#define lua_strlen(L,i)		lua_objlen_hack(L, (i))

#define lua_isfunction(L,n)	(lua_type_hack(L, (n)) == LUA_TFUNCTION)
#define lua_istable(L,n)	(lua_type_hack(L, (n)) == LUA_TTABLE)
#define lua_islightuserdata(L,n)	(lua_type_hack(L, (n)) == LUA_TLIGHTUSERDATA)
#define lua_isnil(L,n)		(lua_type_hack(L, (n)) == LUA_TNIL)
#define lua_isboolean(L,n)	(lua_type_hack(L, (n)) == LUA_TBOOLEAN)
#define lua_isthread(L,n)	(lua_type_hack(L, (n)) == LUA_TTHREAD)
#define lua_isnone(L,n)		(lua_type_hack(L, (n)) == LUA_TNONE)
#define lua_isnoneornil(L, n)	(lua_type_hack(L, (n)) <= 0)

#define lua_pushliteral(L, s)	\
	lua_pushlstring_hack(L, "" s, (sizeof(s)/sizeof(char))-1)

#define lua_setglobal(L,s)	lua_setfield_hack(L, LUA_GLOBALSINDEX, (s))
#define lua_getglobal(L,s)	lua_getfield_hack(L, LUA_GLOBALSINDEX, (s))

#define lua_tostring(L,i)	lua_tolstring_hack(L, (i), NULL)



/*
** compatibility macros and functions
*/

#define lua_open()	luaL_newstate_hack()

#define lua_getregistry(L)	lua_pushvalue_hack(L, LUA_REGISTRYINDEX)

#define lua_getgccount(L)	lua_gc_hack(L, LUA_GCCOUNT, 0)

#define lua_Chunkreader		lua_Reader
#define lua_Chunkwriter		lua_Writer


/* hack */
LUA_API void lua_setlevel	(lua_State *from, lua_State *to);


/*
** {======================================================================
** Debug API
** =======================================================================
*/


/*
** Event codes
*/
#define LUA_HOOKCALL	0
#define LUA_HOOKRET	1
#define LUA_HOOKLINE	2
#define LUA_HOOKCOUNT	3
#define LUA_HOOKTAILRET 4


/*
** Event masks
*/
#define LUA_MASKCALL	(1 << LUA_HOOKCALL)
#define LUA_MASKRET	(1 << LUA_HOOKRET)
#define LUA_MASKLINE	(1 << LUA_HOOKLINE)
#define LUA_MASKCOUNT	(1 << LUA_HOOKCOUNT)

typedef struct lua_Debug lua_Debug;  /* activation record */


/* Functions to be called by the debuger in specific events */
typedef void (*lua_Hook) (lua_State *L, lua_Debug *ar);


LUA_API int lua_getstack_hack (lua_State *L, int level, lua_Debug *ar);
LUA_API int lua_getinfo_hack (lua_State *L, const char *what, lua_Debug *ar);
LUA_API const char *lua_getlocal_hack (lua_State *L, const lua_Debug *ar, int n);
LUA_API const char *lua_setlocal_hack (lua_State *L, const lua_Debug *ar, int n);
LUA_API const char *lua_getupvalue_hack (lua_State *L, int funcindex, int n);
LUA_API const char *lua_setupvalue_hack (lua_State *L, int funcindex, int n);
LUA_API int lua_sethook_hack (lua_State *L, lua_Hook func, int mask, int count);
LUA_API lua_Hook lua_gethook_hack (lua_State *L);
LUA_API int lua_gethookmask_hack (lua_State *L);
LUA_API int lua_gethookcount_hack (lua_State *L);

/* From Lua 5.2. */
LUA_API void *lua_upvalueid_hack (lua_State *L, int idx, int n);
LUA_API void lua_upvaluejoin_hack (lua_State *L, int idx1, int n1, int idx2, int n2);
LUA_API int lua_loadx_hack (lua_State *L, lua_Reader reader, void *dt,
		       const char *chunkname, const char *mode);


struct lua_Debug {
  int event;
  const char *name;	/* (n) */
  const char *namewhat;	/* (n) `global', `local', `field', `method' */
  const char *what;	/* (S) `Lua', `C', `main', `tail' */
  const char *source;	/* (S) */
  int currentline;	/* (l) */
  int nups;		/* (u) number of upvalues */
  int linedefined;	/* (S) */
  int lastlinedefined;	/* (S) */
  char short_src[LUA_IDSIZE]; /* (S) */
  /* private part */
  int i_ci;  /* active function */
};

/* }====================================================================== */


/******************************************************************************
* Copyright (C) 1994-2008 Lua.org, PUC-Rio.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/


#endif

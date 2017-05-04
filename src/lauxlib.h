/*
** $Id: lauxlib.h,v 1.88.1.1 2007/12/27 13:02:25 roberto Exp $
** Auxiliary functions for building Lua libraries
** See Copyright Notice in lua.h
*/


#ifndef lauxlib_h
#define lauxlib_h


#include <stddef.h>
#include <stdio.h>

#include "lua.h"


#define luaL_getn(L,i)          ((int)lua_objlen_hack(L, i))
#define luaL_setn(L,i,j)        ((void)0)  /* no op! */

/* extra error code for `luaL_load' */
#define LUA_ERRFILE     (LUA_ERRERR+1)

typedef struct luaL_Reg {
  const char *name;
  lua_CFunction func;
} luaL_Reg;

LUALIB_API void (luaL_openlib_hack) (lua_State *L, const char *libname,
                                const luaL_Reg *l, int nup);
LUALIB_API void (luaL_register_hack) (lua_State *L, const char *libname,
                                const luaL_Reg *l);
LUALIB_API int (luaL_getmetafield_hack) (lua_State *L, int obj, const char *e);
LUALIB_API int (luaL_callmeta_hack) (lua_State *L, int obj, const char *e);
LUALIB_API int (luaL_typerror_hack) (lua_State *L, int narg, const char *tname);
LUALIB_API int (luaL_argerror_hack) (lua_State *L, int numarg, const char *extramsg);
LUALIB_API const char *(luaL_checklstring_hack) (lua_State *L, int numArg,
                                                          size_t *l);
LUALIB_API const char *(luaL_optlstring_hack) (lua_State *L, int numArg,
                                          const char *def, size_t *l);
LUALIB_API lua_Number (luaL_checknumber_hack) (lua_State *L, int numArg);
LUALIB_API lua_Number (luaL_optnumber_hack) (lua_State *L, int nArg, lua_Number def);

LUALIB_API lua_Integer (luaL_checkinteger_hack) (lua_State *L, int numArg);
LUALIB_API lua_Integer (luaL_optinteger_hack) (lua_State *L, int nArg,
                                          lua_Integer def);

LUALIB_API void (luaL_checkstack_hack) (lua_State *L, int sz, const char *msg);
LUALIB_API void (luaL_checktype_hack) (lua_State *L, int narg, int t);
LUALIB_API void (luaL_checkany_hack) (lua_State *L, int narg);

LUALIB_API int   (luaL_newmetatable_hack) (lua_State *L, const char *tname);
LUALIB_API void *(luaL_checkudata_hack) (lua_State *L, int ud, const char *tname);

LUALIB_API void (luaL_where_hack) (lua_State *L, int lvl);
LUALIB_API int (luaL_error_hack) (lua_State *L, const char *fmt, ...);

LUALIB_API int (luaL_checkoption_hack) (lua_State *L, int narg, const char *def,
                                   const char *const lst[]);

LUALIB_API int (luaL_ref_hack) (lua_State *L, int t);
LUALIB_API void (luaL_unref_hack) (lua_State *L, int t, int ref);

LUALIB_API int (luaL_loadfile_hack) (lua_State *L, const char *filename);
LUALIB_API int (luaL_loadbuffer_hack) (lua_State *L, const char *buff, size_t sz,
                                  const char *name);
LUALIB_API int (luaL_loadstring_hack) (lua_State *L, const char *s);

LUALIB_API lua_State *(luaL_newstate_hack) (void);


LUALIB_API const char *(luaL_gsub_hack) (lua_State *L, const char *s, const char *p,
                                                  const char *r);

LUALIB_API const char *(luaL_findtable_hack) (lua_State *L, int idx,
                                         const char *fname, int szhint);

/* From Lua 5.2. */
LUALIB_API int luaL_fileresult_hack(lua_State *L, int stat, const char *fname);
LUALIB_API int luaL_execresult_hack(lua_State *L, int stat);
LUALIB_API int (luaL_loadfilex_hack) (lua_State *L, const char *filename,
				 const char *mode);
LUALIB_API int (luaL_loadbufferx_hack) (lua_State *L, const char *buff, size_t sz,
				   const char *name, const char *mode);
LUALIB_API void luaL_traceback_hack (lua_State *L, lua_State *L1, const char *msg,
				int level);


/*
** ===============================================================
** some useful macros
** ===============================================================
*/

#define luaL_argcheck(L, cond,numarg,extramsg)	\
		((void)((cond) || luaL_argerror_hack(L, (numarg), (extramsg))))
#define luaL_checkstring(L,n)	(luaL_checklstring_hack(L, (n), NULL))
#define luaL_optstring(L,n,d)	(luaL_optlstring_hack(L, (n), (d), NULL))
#define luaL_checkint(L,n)	((int)luaL_checkinteger_hack(L, (n)))
#define luaL_optint(L,n,d)	((int)luaL_optinteger_hack(L, (n), (d)))
#define luaL_checklong(L,n)	((long)luaL_checkinteger_hack(L, (n)))
#define luaL_optlong(L,n,d)	((long)luaL_optinteger_hack(L, (n), (d)))

#define luaL_typename(L,i)	lua_typename_hack(L, lua_type_hack(L,(i)))

#define luaL_dofile(L, fn) \
	(luaL_loadfile_hack(L, fn) || lua_pcall_hack(L, 0, LUA_MULTRET, 0))

#define luaL_dostring(L, s) \
	(luaL_loadstring_hack(L, s) || lua_pcall_hack(L, 0, LUA_MULTRET, 0))

#define luaL_getmetatable(L,n)	(lua_getfield_hack(L, LUA_REGISTRYINDEX, (n)))

#define luaL_opt(L,f,n,d)	(lua_isnoneornil(L,(n)) ? (d) : f(L,(n)))

/*
** {======================================================
** Generic Buffer manipulation
** =======================================================
*/



typedef struct luaL_Buffer {
  char *p;			/* current position in buffer */
  int lvl;  /* number of strings in the stack (level) */
  lua_State *L;
  char buffer[LUAL_BUFFERSIZE];
} luaL_Buffer;

#define luaL_addchar(B,c) \
  ((void)((B)->p < ((B)->buffer+LUAL_BUFFERSIZE) || luaL_prepbuffer_hack(B)), \
   (*(B)->p++ = (char)(c)))

/* compatibility only */
#define luaL_putchar(B,c)	luaL_addchar(B,c)

#define luaL_addsize(B,n)	((B)->p += (n))

LUALIB_API void (luaL_buffinit_hack) (lua_State *L, luaL_Buffer *B);
LUALIB_API char *(luaL_prepbuffer_hack) (luaL_Buffer *B);
LUALIB_API void (luaL_addlstring_hack) (luaL_Buffer *B, const char *s, size_t l);
LUALIB_API void (luaL_addstring_hack) (luaL_Buffer *B, const char *s);
LUALIB_API void (luaL_addvalue_hack) (luaL_Buffer *B);
LUALIB_API void (luaL_pushresult_hack) (luaL_Buffer *B);


/* }====================================================== */


/* compatibility with ref system */

/* pre-defined references */
#define LUA_NOREF       (-2)
#define LUA_REFNIL      (-1)

#define lua_ref(L,lock) ((lock) ? luaL_ref_hack(L, LUA_REGISTRYINDEX) : \
      (lua_pushstring_hack(L, "unlocked references are obsolete"), lua_error_hack(L), 0))

#define lua_unref(L,ref)        luaL_unref_hack(L, LUA_REGISTRYINDEX, (ref))

#define lua_getref(L,ref)       lua_rawgeti_hack(L, LUA_REGISTRYINDEX, (ref))


#define luaL_reg	luaL_Reg

#endif

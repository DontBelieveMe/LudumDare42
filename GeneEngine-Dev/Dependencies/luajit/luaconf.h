/*
** Configuration header.
** Copyright (C) 2005-2013 Mike Pall. See Copyright Notice in luajit.h
*/

#ifndef luaconf_h
#define luaconf_h

#include <limits.h>
#include <stddef.h>

#define LUAJIT_DISABLE_FFI
#define LUAJIT_ENABLE_LUA52COMPAT
#define LUAJIT_DISABLE_JIT 

#define LUAJIT_USE_SYSMALLOC
#define LUAJIT_USE_VALGRIND
#define LUAJIT_USE_GDBJIT

#define LUA_USE_APICHECK
#define LUA_USE_ASSERT

#define LUAJIT_CPU_SSE2
#define LUAJIT_CPU_NOCMOV

/* Default path for loading Lua and C modules with require(). */
#define LUA_MODULE_SUFFIX "@LUA_MODULE_SUFFIX@"
#define LUA_DIR	"@LUA_DIR@"
#define LUA_LDIR	"@LUA_LDIR@"
#define LUA_CDIR	"@LUA_CDIR@"

#define LUA_PATH_DEFAULT "@LUA_PATH_DEFAULT@"
#define LUA_CPATH_DEFAULT "@LUA_CPATH_DEFAULT@"

/* Environment variable names for path overrides and initialization code. */
#define LUA_PATH "@LUA_PATH@"
#define LUA_CPATH "@LUA_CPATH@"
#define LUA_INIT "@LUA_INIT@"

/* Special file system characters. */
#define LUA_DIRSEP	"@LUA_DIRSEP@"
#define LUA_PATHSEP	";"
#define LUA_PATH_MARK	"?"
#define LUA_EXECDIR	"!"
#define LUA_IGMARK	"-"
#define LUA_PATH_CONFIG \
  LUA_DIRSEP "\n" LUA_PATHSEP "\n" LUA_PATH_MARK "\n" \
  LUA_EXECDIR "\n" LUA_IGMARK

/* Quoting in error messages. */
#define LUA_QL(x)	"'" x "'"
#define LUA_QS		LUA_QL("%s")

/* Various tunables. */
#define LUAI_MAXSTACK	@LUAI_MAXSTACK@	/* Max. # of stack slots for a thread (<64K). */
#define LUAI_MAXCSTACK	@LUAI_MAXCSTACK@	/* Max. # of stack slots for a C func (<10K). */
#define LUAI_GCPAUSE	@LUAI_GCPAUSE@	/* Pause GC until memory is at 200%. */
#define LUAI_GCMUL	@LUAI_GCMUL@	/* Run GC at 200% of allocation speed. */
#define LUA_MAXCAPTURES	@LUA_MAXCAPTURES@	/* Max. pattern captures. */

/* Compatibility with older library function names. */
#define LUA_COMPAT_MOD		/* OLD: math.mod, NEW: math.fmod */
#define LUA_COMPAT_GFIND	/* OLD: string.gfind, NEW: string.gmatch */

/* Configuration for the frontend (the luajit executable). */
#if defined(luajit_c)
#define LUA_PROGNAME	"luajit"  /* Fallback frontend name. */
#define LUA_PROMPT		"@LUA_PROMPT@"  /* Interactive prompt. */
#define LUA_PROMPT2	"@LUA_PROMPT2@"  /* Continuation prompt. */
#define LUA_MAXINPUT	@LUA_MAXINPUT@ /* Max. input line length. */
#endif

/* Note: changing the following defines breaks the Lua 5.1 ABI. */
#define LUA_INTEGER	ptrdiff_t
#define LUA_IDSIZE	60	/* Size of lua_Debug.short_src. */
/*
** Size of lauxlib and io.* on-stack buffers. Weird workaround to avoid using
** unreasonable amounts of stack space, but still retain ABI compatibility.
** Blame Lua for depending on BUFSIZ in the ABI, blame **** for wrecking it.
*/
#define LUAL_BUFFERSIZE	(BUFSIZ > 16384 ? 8192 : BUFSIZ)

/* The following defines are here only for compatibility with luaconf.h
** from the standard Lua distribution. They must not be changed for LuaJIT.
*/
#define LUA_NUMBER_DOUBLE
#define LUA_NUMBER		double
#define LUAI_UACNUMBER		double
#define LUA_NUMBER_SCAN		"%lf"
#define LUA_NUMBER_FMT		"%.14g"
#define lua_number2str(s, n)	sprintf((s), LUA_NUMBER_FMT, (n))
#define LUAI_MAXNUMBER2STR	32
#define LUA_INTFRMLEN		"l"
#define LUA_INTFRM_T		long

/* Linkage of public API functions. */
#if defined(LUA_BUILD_AS_DLL)
#if defined(LUA_CORE) || defined(LUA_LIB)
#define LUA_API		__declspec(dllexport)
#else
#define LUA_API		__declspec(dllimport)
#endif
#else
#define LUA_API		extern
#endif

#define LUALIB_API	LUA_API

/* Support for internal assertions. */
#if defined(LUA_USE_ASSERT) || defined(LUA_USE_APICHECK)
#include <assert.h>
#endif
#ifdef LUA_USE_ASSERT
#define lua_assert(x)		assert(x)
#endif
#ifdef LUA_USE_APICHECK
#define luai_apicheck(L, o)	{ (void)L; assert(o); }
#else
#define luai_apicheck(L, o)	{ (void)L; }
#endif

#endif

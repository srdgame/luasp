#ifndef __LUASP_EXT_HEADER_
#define __LUASP_EXT_HEADER_

#ifdef __cplusplus 
extern "C" {
#endif /*__cplusplus*/ 

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#ifdef __cplusplus 
} 
#endif /*__cplusplus*/

#ifndef LUA_MODULE_VERSION
#define LUA_MODULE_VERSION "0.1.0"
#endif

#ifndef LUA_MODULE
#define __DEF_LUASP_MODULE(X)\
	int luaopen_##X(lua_State* L); \
	void luaclose_##X(); \
	const char* luaversion_##X(void);
#else
#define __DEF_LUASP_MODULE(X) \
	int luaopen_##X(lua_State* L); \
	const char* luaversion_##X(void);
#endif /*LUA_MODULE*/

#ifdef __cplusplus 
#define DEF_LUASP_MODULE(X) \
extern "C" { \
	__DEF_LUASP_MODULE(X)\
}
#else
#define DEF_LUASP_MODULE(X) \
	__DEF_LUASP_MODULE(X)
#endif /*__cplusplus*/

#define IMPL_LUASP_MODULE(X) \
	const char* luaversion_##X(void) { return LUA_MODULE_VERSION; };

#endif /*ifndef LUASP_HEADER_H_*/


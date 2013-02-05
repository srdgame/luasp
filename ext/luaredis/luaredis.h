#ifndef __LUAREDIS_H
#define __LUAREDIS_H


#ifdef __cplusplus 
extern "C" {
#endif /*__cplusplus*/ 

#include <lua.h>  
#include <lauxlib.h>  
#include <lualib.h>  

int luaopen_luaredis(lua_State* L);

#ifndef LUA_MODULE
void luaclose_luamredis(void);
#endif /*LUA_MODULE*/

const char* luaversion_luaredis(void);


#ifdef __cplusplus 
}
#endif /*__cplusplus*/ 


#endif /*__LUAREDIS_H*/

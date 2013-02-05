
/* 
 * Copyright (C) Dirk Chang
 */

#include "luaredis.h"
#include <cstdlib>

void luaclose_luaredis(void)
{
}


int luaopen_luaredis(lua_State* L)
{
	luaL_dostring(L, "redis = require 'redis'");
#ifdef LUA_MODULE
    atexit(luaclose_luaredis);
#endif /*LUA_MODULE*/

    return 0;
}



#include <algorithm>
#include <uuid/uuid.h>
#include "lspcgi.h"
#include "llsplib.h"

int lsp::lua_log(lua_State *L)
{
    const char* s=luaL_checkstring(L,1);

    //log(LOG_INFO, "%s", s);
    FCGX_Request* r = (FCGX_Request*)luaL_lsp_get_io_ctx(L);
	if (r)
	{
		FCGX_PutS(s, r->err);
	}
    
    return 0;
}

/*
int lsp::lua_content_type(lua_State *L)
{
    FCGX_Request* r = (FCGX_Request*)luaL_lsp_get_io_ctx(L);

    const char* s=luaL_checkstring(L,1);

    //apr->content_type=apr_pstrdup(apr->pool,s);
	//todo:set content_type
	
    
    return 0;
}
*/
/*
int lsp::lua_set_out_header(lua_State *L)
{
    FCGX_Request* r = (FCGX_Request*)luaL_lsp_get_io_ctx(L);

    const char* s1=luaL_checkstring(L,1);
    const char* s2=0;
    
    if(lua_gettop(L)>1)
		s2=luaL_checkstring(L,2);

    if(!s2 || !*s2)
		header_table_unset(r,s1);
    else
		header_table_set(r,s1,s2);

    return 0;
}
*/
int lsp::lua_get_in_header(lua_State *L)
{
    FCGX_Request* r = (FCGX_Request*)luaL_lsp_get_io_ctx(L);

    const char* s1=luaL_checkstring(L,1);

	std::string str = "HTTP_";
	str += s1;
	std::transform(str.begin(), str.end(),str.begin(), ::toupper);

    const char* s2=FCGX_GetParam(str.c_str(), r->envp);

    if(!s2)
		s2="";

    lua_pushstring(L,s2);
 
    return 1;
}


int lsp::lua_uuid_gen(lua_State *L)
{
    char tmp[64];

    uuid_t uuid;
    uuid_generate(uuid);
    uuid_unparse(uuid,tmp);
    
    lua_pushstring(L,tmp);
 
    return 1;
}

int lsp::lua_version(lua_State *L)
{
    lua_pushstring(L,LSP_VERSION);
 
    return 1;
}

int lsp::lua_module_version(lua_State *L)
{
    const char* name = luaL_checkstring(L, 1);
	if (!name)
		return 0;

	const char* version = mod_mgr.version(name);
	if (!version)
		return 0;
	lua_pushstring(L, version);

	return 1;
}

int lsp::lua_module_list(lua_State *L)
{
	return mod_mgr.list(L);
}


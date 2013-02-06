
#include <algorithm>
#include <sstream>
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

int lsp::lua_content_type(lua_State *L)
{
    FCGX_Request* r = (FCGX_Request*)luaL_lsp_get_io_ctx(L);

    const char* s=luaL_checkstring(L,1);

	if (s)
		header_table_set(r, "CONTENT-TYPE", s);
	else
		header_table_unset(r, "CONTENT-TYPE");

    return 0;
}

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

static inline void output_headers(lsp::REQBAG* bag)
{
	if (bag->header_out)
		return;
	std::map<std::string, std::string>::iterator ptr = bag->out_header.begin();
	std::stringstream ss;
	for (; ptr != bag->out_header.end(); ++ptr)
	{
		ss << ptr->first << ": ";
		ss << ptr->second << std::endl;
	}
	FCGX_PutS(ss.str().c_str(), bag->out);
	bag->header_out = true;
}
int lsp::io_def_puts(void* ctx,const char* s) { 
	REQBAG *bag = (REQBAG*)ctx;
	
	output_headers(bag);
	return FCGX_PutS(s, bag->out); 
}

int lsp::io_def_putc(void* ctx,int c) {
	REQBAG *bag = (REQBAG*)ctx;
	
	output_headers(bag);
	return FCGX_PutChar(c, bag->out);
}

int lsp::io_def_write(void* ctx,const char* s,size_t len) {
	REQBAG *bag = (REQBAG*)ctx;
	
	output_headers(bag);
	return FCGX_PutStr(s, len, bag->out); 
}

int lsp::header_table_set(FCGX_Request* r, const char* key, const char* value)
{
	REQBAG* bag = (REQBAG*)r;
	bag->out_header[key] = value;
}

int lsp::header_table_unset(FCGX_Request* r, const char* key)
{
	REQBAG* bag = (REQBAG*)r;
	bag->out_header.erase(key);
}


/*
 * file: main.cpp
 *
 */
#ifndef lint
static const char rcsid[] = "$Id: echo-x.c,v 1.1 2001/06/19 15:06:17 robs Exp $";
#endif /* not lint */

#include <cstdlib>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <uuid/uuid.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "llsplib.h"

#ifndef LSP_VERSION
#define LSP_VERSION ""
#endif /*LSP_VERSION*/

#include "lspcgi.h"

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif

#define luasp_config_file "/etc/luasp/luasp.conf"

void load_configuration();

lsp::ExtModMgr lsp::mod_mgr;

lsp::LSP_CONF* lsp::g_conf = NULL;

int main ()
{
	lsp::g_conf = new lsp::LSP_CONF();
	load_configuration();
	// load ext module first
	lsp::mod_mgr.load();

	FCGX_Init();

	lsp::REQBAG request;

	FCGX_InitRequest(&request, 0, 0);

    openlog("luasp_fcgi", LOG_CONS | LOG_PID, 0);
    log(LOG_INFO, "%s", "luasp_fcgi start working...");

    while (FCGX_Accept_r(&request) >= 0) {
		lsp::LUABAG luabag;
		if (!luabag_init(&luabag))
		{
	//		log(LOG_INFO, "%s", "lua_fcgi init failed!!");
		}
		int result = luabag_run(&luabag, &request);
		if (0 != result)
		{
			output_error_page(&request, result);
	//		log(LOG_INFO, "%s", "lua_fcgi run script failed!!");
		}
		if (!luabag_cleanup(&luabag))
		{
		//	log(LOG_INFO, "%s", "lua_fcgi cleanup failed!!!");
		}
		FCGX_Finish_r(&request);
    } /* while */
    log(LOG_INFO, "%s", "luasp_fcgi stoped!");

	delete lsp::g_conf;
    return 0;
}

void load_configuration()
{
	Config conf(luasp_config_file, environ);
	lsp::g_conf->init_script = conf.pString("init_script");
	lsp::g_conf->cookie_name = conf.pString("cookie_name");
	lsp::g_conf->cookie_days = conf.pInt("cookie_days");
	lsp::g_conf->cookie_path = conf.pString("cookie_path");
	lsp::g_conf->show_exception = conf.pInt("show_exception");
}

bool lsp::luabag_init(LUABAG *luabag)
{
    if(!luabag)
    {
		return false;
	}

	luabag->L=lua_open();
	
	luaL_openlibs(luabag->L);
	
	luaopen_lualsp(luabag->L);

	mod_mgr.open(luabag->L);
	
	lua_register(luabag->L,"module_version",lua_version);
	lua_register(luabag->L,"log",lua_log);
	lua_register(luabag->L,"uuid_gen",lua_uuid_gen);

	if(!g_conf->init_script.empty())
	{
	    if(luaL_loadfile(luabag->L, g_conf->init_script.c_str()) || lua_pcall(luabag->L,0,0,0))
	    {
			log(LOG_ERR, "%s", lua_tostring(luabag->L,-1));
			lua_pop(luabag->L,1);
		}	    
	}
	
	/*	
	lua_register(luabag->L,"content_type",lua_content_type);
	lua_register(luabag->L,"set_out_header",lua_set_out_header);
	*/
	lua_register(luabag->L,"get_in_header",lua_get_in_header);

	return true;
}

int lsp::luabag_run(LUABAG* luabag, FCGX_Request* r)
{
	int handler_type = handler_type_unknown;

	char* handler = FCGX_GetParam("SCRIPT_NAME", r->envp);

	if(handler)
    {
		handler=strrchr(handler,'.');
		if(!handler)
		{
			log(LOG_ERR, "%s", "Script name has no extension name");
			return -1;
		}
		if(!strcmp(handler,".lsp") || !strcmp(handler,".lp") ) 
			handler_type = handler_type_lsp;
		else if(!strcmp(handler,".lua"))
			handler_type = handler_type_lua;
		else
		{
			log(LOG_ERR, "%s:%s", "Not correct extension name, valid extension are lp, lsp, lua", handler);
			return -1;
		}
    }
	else
	{
		log(LOG_ERR, "%s", "Not script name");
		return -1;
	}

    lsp_io lio={r, lputs:io_def_puts, lputc:io_def_putc, lwrite:io_def_write};

    luaL_lsp_set_io(luabag->L,&lio);

	char* p = FCGX_GetParam("REQUEST_METHOD", r->envp);
	if(p && !strcmp(p,"POST"))
    {
		int rc = read_request_data(luabag->L);
		if(rc!= 0)
			return rc;
    }

	p = FCGX_GetParam("QUERY_STRING", r->envp);
    luaL_lsp_setargs(luabag->L, p, p ? strlen(p) : 0);

    lua_getfield(luabag->L,LUA_GLOBALSINDEX,"env");    
    //luaL_lsp_setfield(luabag->L,"server_admin",r->server->server_admin);
    luaL_lsp_setfield(luabag->L,"server_hostname", FCGX_GetParam("SERVER_NAME", r->envp));
    luaL_lsp_setfield(luabag->L,"remote_ip",FCGX_GetParam("REMOTE_ADDR", r->envp));
    luaL_lsp_setfield(luabag->L,"remote_host",FCGX_GetParam("REMOTE_ADDR", r->envp));
    luaL_lsp_setfield(luabag->L,"remote_port",FCGX_GetParam("REMOTE_PORT", r->envp));
    luaL_lsp_setfield(luabag->L,"local_ip", FCGX_GetParam("SERVER_ADDR", r->envp));
	luaL_lsp_setfield(luabag->L,"local_host",FCGX_GetParam("SERVER_NAME", r->envp));
    luaL_lsp_setfield(luabag->L,"local_port",FCGX_GetParam("SERVER_PORT", r->envp));
	char hostname[256];
	if (0 == gethostname(hostname, 256))
	{
		luaL_lsp_setfield(luabag->L,"local_host", hostname);
		luaL_lsp_setfield(luabag->L,"hostname", hostname);
	}
    luaL_lsp_setfield(luabag->L,"method", FCGX_GetParam("REQUEST_METHOD", r->envp));
    luaL_lsp_setfield(luabag->L,"handler", handler);
    luaL_lsp_setfield(luabag->L,"uri", FCGX_GetParam("REQUEST_URI", r->envp));
    luaL_lsp_setfield(luabag->L,"doc_uri", FCGX_GetParam("DOCUMENT_URI", r->envp));
    luaL_lsp_setfield(luabag->L,"doc_root", FCGX_GetParam("DOCUMENT_ROOT", r->envp));
	char* filename = FCGX_GetParam("SCRIPT_FILENAME", r->envp);
    luaL_lsp_setfield(luabag->L,"filename", filename);
    luaL_lsp_setfield(luabag->L,"args", FCGX_GetParam("QUERY_STRING", r->envp));
    luaL_lsp_setfield(luabag->L,"accept_lang", FCGX_GetParam("HTTP_ACCEPT_LANGUAGE", r->envp));
    lua_pop(luabag->L,1);
    
    luaL_lsp_chdir_to_file(luabag->L, filename);
    
    luaL_lsp_session_init(luabag->L,
			!g_conf->cookie_name.empty() ? g_conf->cookie_name.c_str() : "LSPSESSID",
			g_conf->cookie_days > 0 ? g_conf->cookie_days : 7,
			!g_conf->cookie_path.empty() ? g_conf->cookie_path.c_str() : "/");
    
    int status=0;
    
    switch(handler_type)
    {
		case handler_type_lsp: 
			status=luaL_load_lsp_file(luabag->L, filename); 
			//r->content_type="text/html";
			break;
		case handler_type_lua:
			status=luaL_loadfile(luabag->L, filename);
			//r->content_type="text/plain";
			break;
    }

    if(status)
    {
		const char* e=lua_tostring(luabag->L,-1);
		FCGX_PutS(e, r->err);

		log(LOG_ERR, "%s", e);

        lua_pop(luabag->L,1);
		luaL_lsp_chdir_restore(luabag->L);
		return 500;
    }

    status=lua_pcall(luabag->L,0,LUA_MULTRET,0);
	// won't return other than 0, as the code has been excuted with outputs
    
    if(status)
    {
		const char* e=lua_tostring(luabag->L,-1);

		log(LOG_ERR, "%s", e);
		FCGX_PutS(e, r->err);

		if(g_conf->show_exception)	// if not 0
			FCGX_PutS(e, r->err);

		lua_pop(luabag->L,1);
    }
    

    int rnum=lua_gettop(luabag->L);

    int result = 0;
    
    if(rnum>0)
    {
		result = lua_tointeger(luabag->L,-1);

		if(!result || result==200)
			result = 0;

		lua_pop(luabag->L,rnum);
    }

    luaL_lsp_chdir_restore(luabag->L);

	/*
    if(result == 0)
		FCGX_FFlush(r->out);
	else
		FCGX_FFlush(r->err);
		*/

    return result;
}


bool lsp::luabag_cleanup(LUABAG* luabag)
{
    if(luabag && luabag->L)
    {
		//lsp::mod_mgr.close(luabag->L);
		lua_close(luabag->L);
		luabag->L=0;
    }

    return true;
}



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


int lsp::read_request_data(lua_State *L)
{
    FCGX_Request* r = (FCGX_Request*)luaL_lsp_get_io_ctx(L);

    const char* p=FCGX_GetParam("CONTENT_LENGTH", r->envp);
    int content_length = p? atoi(p) : -1;

    if(content_length < 0)
		return 411;//HTTP_LENGTH_REQUIRED;

	// TODO: for max post?
    if(content_length > 4096)
		return 400;//HTTP_BAD_REQUEST;

    int retval = 0;

    luaL_Buffer buf;
    luaL_buffinit(L,&buf);

    //if(ap_should_client_block(apr))
    {
		char tmp[1024];
        int len = 0;
	
        while(len<content_length)
        {
            int n = content_length - len;

            //n = ap_get_client_block(apr,tmp,n>sizeof(tmp)?sizeof(tmp):n);
			n = FCGX_GetStr(tmp, n > sizeof(tmp) ? sizeof(tmp) : n, r->in);
            if(n <= 0)
                break;

            len+=n;
			luaL_addlstring(&buf,tmp,n);
        }

		if(len!=content_length)
			retval = -1;//HTTP_REQUEST_TIME_OUT;
    }

    const char* content_type = FCGX_GetParam("CONTENT_TYPE", r->envp);

    int n = lua_gettop(L);

    if(content_type && !strcmp(content_type,"application/x-www-form-urlencoded"))
    {
		lua_getglobal(L,"args_decode");
		luaL_pushresult(&buf);

		if(lua_isfunction(L,-2) && !lua_pcall(L,1,1,0))
			lua_setglobal(L,"args_post");
	}
	else
	{
		lua_getfield(L,LUA_GLOBALSINDEX,"env");
		luaL_pushresult(&buf);

		if(lua_istable(L,-2))
			lua_setfield(L,-2,"content");
	}

	lua_pop(L,lua_gettop(L)-n);
    
    return retval;
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


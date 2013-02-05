
#ifndef LSP_CGI_H
#define LSP_CGI_H

#include <string>

#include "fcgi_config.h"
#include "fcgiapp.h"

#include "log.h"
#include "extmod.h"
#include "config.h"

namespace lsp
{
	// lsp handlers
    enum
    {
		handler_type_unknown=0,
		handler_type_lsp=1,
		handler_type_lua=2
	};
    
	// module loader
	extern ExtModMgr mod_mgr;
	// configureation file
	struct LSP_CONF
	{
		std::string init_script;
		std::string cookie_name;
		int cookie_days;
		std::string cookie_path;
		int show_exception;
	};
	extern LSP_CONF *g_conf;

    // lua container
    struct LUABAG
    {
	lua_State* L;
    };
	struct REQBAG : public FCGX_Request
	{
		std::map<std::string, std::string> out_header;
	};

	inline int io_def_puts(void* ctx,const char* s) { return FCGX_PutS(s, ((FCGX_Request*)ctx)->out); }
    inline int io_def_putc(void* ctx,int c) { return FCGX_PutChar(c, ((FCGX_Request*)ctx)->out); }
    inline int io_def_write(void* ctx,const char* s,size_t len) { return FCGX_PutStr(s, len, ((FCGX_Request*)ctx)->out); }

	// initialize
	bool luabag_init(LUABAG* p);
    // cleanup
    bool luabag_cleanup(LUABAG* p);

	// run
	int luabag_run(LUABAG* p, FCGX_Request* r);

    // implementation of lsp host functions
    int lua_log(lua_State *L);
    int lua_content_type(lua_State *L);
    int lua_set_out_header(lua_State *L);
    int lua_get_in_header(lua_State *L);
    int lua_uuid_gen(lua_State *L);
    int lua_version(lua_State *L);

    // lsp support
    int read_request_data(lua_State *L);

	int header_table_set(FCGX_Request* r, const char* key, const char* value);
	int header_table_unset(FCGX_Request* r, const char* key);

	void output_error_page(FCGX_Request* r, int err, const char* info = NULL);
}


#endif 

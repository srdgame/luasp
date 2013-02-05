
#ifndef LUASP_EXT_MOD_H
#define LUASP_EXT_MOD_H

#include <vector>
#include <string>

#ifdef __cplusplus 
extern "C" {
#endif /*__cplusplus*/ 

#include <lua.h>  
#include <lauxlib.h>  
#include <lualib.h>  

typedef int (*open_extmod)(lua_State* L);
//typedef void (*close_extmod)(void);
typedef const char* (*version_extmod)(void); 

#ifdef __cplusplus 
}
#endif /*__cplusplus*/ 

namespace lsp
{
	struct ExtModule
	{
		open_extmod open;
//		close_extmod close;
		version_extmod version;
		std::string name;
	};

	class ExtModMgr
	{
	public:
		ExtModMgr();
		~ExtModMgr();
	public:
		// load all avaiable module information.
		void load();

		// Load module into LuaState
		void open(lua_State* L);
		// Unload module from luastate
//		void close(lua_State* L);
//
		const char* version(const char* name);
		int list(lua_State* L);

	private:
		std::vector<ExtModule> modules;
	};
}

#endif

/** Copy right **/

#if defined(WIN32) || defined(WIN64)
#include <io.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <dlfcn.h>
#endif
#include <string>
#include <cstring>

#include "extmod.h"
#include "log.h"

const char* mod_lib_folder = "/usr/lib/luasp/";

lsp::ExtModMgr::ExtModMgr()
{
	
}

lsp::ExtModMgr::~ExtModMgr()
{
}

static bool load_mod(const char* path, const char* filename, lsp::ExtModule& mod)
{
	mod.name = filename;

	void* dl = NULL;
	std::string full_path(path);
	full_path += filename;

	dl = dlopen(full_path.c_str(), RTLD_NOW | RTLD_LOCAL);
	if (!dl)
		return false;

	std::string open_func("luaopen_");
	open_func += filename;
	open_func = open_func.substr(0, open_func.rfind('.'));
	mod.open = (open_extmod)dlsym(dl, open_func.c_str());

	/*
	std::string close_func("luaclose_");
	close_func += filename;
	mod.close = (close_extmod)dlsym(dl, close_func.c_str());

	dlclose(dl);
	*/

	return mod.open /* && mod.close */;
}

#if defined(WIN32) || defined(WIN64)
void lsp::ExtModMgr::load()
{
	modules.clear();

	std::string filter = mod_lib_folder;
	filter += "*.so";

	_finddata_t file; 

    if(-1 == _findfirst(filter.c_str(), &file))
	{
		log(LOG_INFO, "no module file found in %s", mod_lib_folder);
	}
    else 
    { 
        do{ 
            if(file.attrib != _A_SUBDIR) 
			{
				lsp::ExtModule mod;
				if (load_mod(mod_lib_folder, file.name, mod))
				{
					modules.push_back(mod);
					log(LOG_INFO, "successfully loaded module %s", file.name);
				}
				else
				{
					log(LOG_WARN, "incompatible module %s - %s", file.name, dlerror());
				}
			}
        } while( _findnext( lf, &file ) == 0 );

		_findclose(lf);
    } 
}
#else
//判断是否为目录
bool is_dir(const char *path)
{
    struct stat statbuf;
    if(lstat(path, &statbuf) ==0)//lstat返回文件的信息，文件信息存放在stat结构中
    {
        return S_ISDIR(statbuf.st_mode) != 0;//S_ISDIR宏，判断文件类型是否为目录
    }
    return false;
}

//判断是否为常规文件
bool is_file(const char *path)
{
    struct stat statbuf;
    if(lstat(path, &statbuf) ==0)
        return S_ISREG(statbuf.st_mode) != 0;//判断文件是否为常规文件
    return false;
}

//判断是否是特殊目录
bool is_special_dir(const char *path)
{
    return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}

//生成完整的文件路径
void get_file_path(const char *path, const char *file_name,  char *file_path)
{
    strcpy(file_path, path);
    if(file_path[strlen(path) - 1] != '/')
        strcat(file_path, "/");
    strcat(file_path, file_name);
}

void lsp::ExtModMgr::load()
{
	modules.clear();

    DIR *dir;
    dirent *dir_info;
    char file_path[PATH_MAX];

	if((dir = opendir(mod_lib_folder)) == NULL)
		return;

	while((dir_info = readdir(dir)) != NULL)
	{
		get_file_path(mod_lib_folder, dir_info->d_name, file_path);
		if(!is_special_dir(dir_info->d_name))
		{
			lsp::ExtModule mod;
			if (load_mod(mod_lib_folder, dir_info->d_name, mod))
			{
				modules.push_back(mod);
				log(LOG_INFO, "successfully loaded module %s", dir_info->d_name);
			}
			else
			{
				log(LOG_WARNING, "incompatible module %s - %s", dir_info->d_name, dlerror());
			}
		}
	}
} 
#endif

void lsp::ExtModMgr::open(lua_State* L)
{
	std::vector<ExtModule>::iterator ptr = modules.begin();
	for(; ptr != modules.end(); ++ptr)
	{
		if (0 != ptr->open(L))
		{
			log(LOG_WARNING, "cannot open module %s for state", ptr->name.c_str());
		}
	}
}
/*
void lsp::ExtModMgr::close(lua_State* L)
{
	std::vector<ExtModule>::iterator ptr = modules.begin();
	for(; ptr != modules.end(); ++ptr)
	{
		ptr->close();
	}
}
*/

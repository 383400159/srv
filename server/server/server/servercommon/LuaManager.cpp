#include <boost/progress.hpp> 
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#ifdef _WIN32

#include <windows.h>
#include <dbghelp.h>
#include <string>
#include <time.h>
#include <sys/stat.h>
#include <algorithm>
#include "exceptdump.h"
#endif


#include "luamanager.hpp"
#include "scriptmanager.hpp"
#include "tolua++.h"

#include "scriptmanager.hpp"

//º”‘ÿlua µ˜”√c++
extern int tolua_server_open (lua_State* tolua_S);


#include <iostream>

class progress_display
{
public:
	progress_display(   unsigned long expected_count,
		std::ostream & os = std::cout)
		: progress_(0.0), now_count_(0), expected_count_(expected_count), os_(os)
	{
		restart(expected_count_); 
	}

	void            restart(unsigned long expected_count)
	{
		progress_ = 0.0; 
		now_count_= 0; 
		expected_count_ = expected_count; 
		os_ << "0%   10   20   30   40   50   60   70   80   90   100%\n"
			<<  "|----|----|----|----|----|----|----|----|----|----|"
			<< std::endl;          
		if ( !expected_count_ ) expected_count_ = 1;  // prevent divide by zero
	}

	unsigned long   operator+=(unsigned long increment)
	{
		now_count_ += increment; 
		progress_ = now_count_ * 1.0 /expected_count_; 
		if(progress_ <= 1.0)
		{ 
			int bar_width = 49; 
			os_ << "["; 
			int pos = bar_width*progress_; 
			for(int i = 0; i < bar_width; ++i)
			{
				if(i < pos) os_ << "="; 
				else if (i == pos) os_ << ">"; 
				else os_ << " "; 
			}
			os_ << "] \r";
			os_ << std::flush; 
			if (progress_ == 1.0)
				os_ << std::endl; 
		}
		return now_count_; 
	}
	unsigned long   operator++()    { return operator+=( 1 ); }
private:
	float           progress_; 
	unsigned long   now_count_; 
	unsigned long   expected_count_; 
	std::ostream&   os_; 
}; 

LuaManager::LuaManager()
	: state_(NULL)
{
}

LuaManager::~LuaManager()
{
	if (NULL != state_)
	{
		lua_close(state_); state_ = NULL; 
	}
}

bool LuaManager::Init(lua_State *new_L, const char * lua_dir, const char *data_dir)
{
	if (NULL != state_) lua_close(state_);
	
	state_ = new_L;

	luaL_openlibs(state_);
	luaopen_pb(state_);
	tolua_server_open(state_);

	std::string lua_src_dir(lua_dir);
	if (!CallPreMain(lua_dir, data_dir)) return false;
	if (!LoadScripts(lua_src_dir + "/src", "")) return false;
	if (!CheckRes(lua_dir)) return false;

	return true;
}

bool LuaManager::DoFile(lua_State *L, const char *path)
{
	if (luaL_loadfile(L, path))
	{
		const char *error_msg = lua_tostring(L, -1);
		printf("LuaManager::DoFile(%s) [ERROR:%s] \n", path, error_msg);
		lua_pop(L, 1);

		return false;
	}

	if (lua_pcall(L, 0, 0, 0))       
	{
		const char *error_msg = lua_tostring(L, -1);
		printf("LuaManager::DoFile(%s) [ERROR:%s] \n", path, error_msg);
		lua_pop(L, 1);

		return false;
	}

	return true;
}

bool LuaManager::Call(int nargs)
{
	if (lua_pcall(state_, nargs, 0, 0))
	{
		const char *error_msg = lua_tostring(state_, -1);
		printf("LuaManager::Call [ERROR:%s] \n", error_msg);

		lua_pop(state_, 1);

		return false;
	}

	return true;
}

void LuaManager::PushNum(double d)
{
	tolua_pushnumber(state_, (lua_Number)d);
}

void LuaManager::PushUserType(const char *type, void *data)
{
	tolua_pushusertype(state_, data, type);
}

void LuaManager::PushString(const char *s)
{
	tolua_pushstring(state_, s);
}

void LuaManager::PushLString(const char *s, unsigned int length){
	tolua_pushlstring(state_, s, length);
}

bool LuaManager::CallPreMain(const std::string &script_path, const std::string &data_dir){
	if (!DoFile(state_, (script_path + "/premain.lua").c_str()))return false;

	if (!sScriptManager.PCall("add_path_to_env", LARG_STRING, (";" + script_path + "/?.lua").c_str(), LARG_END))
		return false;

	if (!sScriptManager.PCall("add_path_to_env", LARG_STRING, (";" + script_path + "/src/?.lua").c_str(), LARG_END))
		return false;

	/*if (!sScriptManager.PCall("add_path_to_env", LARG_STRING, (";" + script_path + "/src/protocol/?.lua").c_str(), LARG_END))
		return false;

	if (!sScriptManager.PCall("add_path_to_env", LARG_STRING, (";" + script_path + "/src/db_param/?.lua").c_str(), LARG_END))
		return false;*/

	if (!sScriptManager.PCall("add_path_to_env", LARG_STRING, (";" + data_dir + "/?.lua").c_str(), LARG_END))
		return false;

	if (!sScriptManager.PCall("add_path_to_env", LARG_STRING, ";../scriptcommon/proto-gen-lua/?.lua", LARG_END))
		return false;

	if (!sScriptManager.PCall("add_path_to_env", LARG_STRING, ";../scriptcommon/rpc/?.lua", LARG_END))
		return false;

	if (!sScriptManager.PCall("add_path_to_env", LARG_STRING, ";../scriptcommon/util/?.lua", LARG_END))
		return false;

	return true;
}

static uint32_t CalLuaFileCount(const std::string &script_path, const std::string &rec_path, bool is_rec)
{
	using namespace boost::filesystem;
	using namespace std;

	static uint16_t script_file_count = 0;
	path p (script_path);   // p reads clearer than argv[1] in the following code
	try
	{
		if (exists(p))    // does p actually exist?
		{
			if (is_directory(p))      // is p a directory?
			{
				typedef vector<path> vec;             // store paths,
				vec v;                                // so we can sort them later

				copy(directory_iterator(p), directory_iterator(), back_inserter(v));

				if (!is_rec) script_file_count = 0;
				for (vec::const_iterator it (v.begin()); it != v.end(); ++it)
				{
					if (is_directory(*it)){
						if (!CalLuaFileCount(it->string(), (is_rec ? rec_path + "/" : "") + it->stem().string(), true))
							return false;
						else
							continue;
					}
					if (strcmp(it->extension().string().c_str(), ".lua"))
						continue;

					++script_file_count;
				}
			}

			else
			{
				cout <<  "total lua file count:" << script_file_count << endl;
			}
		}
		else
		{
			cout << "\n" << p << " does not exist" << endl;
		}
	}

	catch (const filesystem_error& ex)
	{
		cout << ex.what() << endl;
	}

	return script_file_count;
}

bool LuaManager::LoadScripts(const std::string &script_path, const std::string &rec_path, bool is_rec)
{
	using namespace boost::filesystem;
	using namespace std;

	if (!is_rec)
	{
#ifdef WIN32
		Sleep(1000);
#else
		sleep(1);
#endif
		cout << "loading script..." << endl;
	}
	std::string temp_string = script_path;
	std::string temp_rec_path = rec_path;
	static progress_display pd(CalLuaFileCount(temp_string, temp_rec_path, is_rec), std::cout); 
	//static boost::progress_display pd(CalLuaFileCount(temp_string, temp_rec_path, is_rec), std::cout, "", "", "");
	static bool is_first_load = true;

	if (!is_rec && !is_first_load){
		pd.restart(CalLuaFileCount(temp_string, temp_rec_path, is_rec));
	} else {
		is_first_load = false;
	}

	path p (script_path);   // p reads clearer than argv[1] in the following code
	try
	{
		if (exists(p))    // does p actually exist?
		{
			if (is_directory(p))      // is p a directory?
			{
				typedef vector<path> vec;             // store paths,
				vec v;                                // so we can sort them later

				copy(directory_iterator(p), directory_iterator(), back_inserter(v));

				using boost::progress_display;
				
				static uint16_t script_file_count = 0;
				if (!is_rec) script_file_count = 0;
				for (vec::const_iterator it (v.begin()); it != v.end(); ++it)
				{
					if (is_directory(*it)){
						if (!LoadScripts(it->string(), (is_rec ? rec_path + "/" : "") + it->stem().string(), true))
							return false;
						else
							continue;
					}
					if (strcmp(it->extension().string().c_str(), ".lua"))
						continue;

					if (!sScriptManager.PCall("require_ex", LARG_STRING, ((is_rec ? rec_path + "/" : "") + it->stem().string()).c_str(), LARG_END))
						return false;

					++pd;
					++script_file_count;
				}

				if (!is_rec)
				cout <<  "total lua file count:" << script_file_count << endl;
			}

			else
			{
				cout << "\n" << p << " is not a directory" << endl;
			}
		}
		else
		{
			cout << "\n" << p << " does not exist" << endl;
		}
	}

	catch (const filesystem_error& ex)
	{
		cout << ex.what() << endl;
	}
	
	return true;
}

bool LuaManager::CallMain(const std::string &script_path){
	if (!DoFile(state_, (script_path + "/main.lua").c_str()))return false;

	return true;
}

bool LuaManager::CheckRes(const std::string &script_path)
{
	if (!DoFile(state_, (script_path + "/res_check.lua").c_str()))return false;

	return true;
}

bool LuaManager::ReloadAll(const std::string &script_path)
{
	if (!LoadScripts(script_path + "/src", "")) return false;
	if (!CheckRes(script_path)) return false;
	return true;
}
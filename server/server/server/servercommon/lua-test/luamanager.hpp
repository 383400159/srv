#ifndef __LUA_MANAGER_HPP__
#define __LUA_MANAGER_HPP__

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
	int luaopen_pb (lua_State *L);
}

enum
{
	LARG_INT32 = 0,
	LARG_UINT32,
	LARG_DOUBLE,
	LARG_UINT64,
	LARG_STRING,
	LARG_USER_TYPE,
	LARG_LSTRING,
	LARG_END = 0x7FFFFFFF,         
};

static const int SCRIPT_MAX_LARG_NUM = 8;						// 最多往脚本传递8个参数

class LuaManager
{
public:
	LuaManager();
	~LuaManager();

	static bool DoFile(lua_State *L, const char *path);

	bool Init(lua_State *new_L, const char * lua_dir, const char *data_dir);
	bool CallMain(const std::string &script_path);
	bool CheckRes(const std::string &script_path);
	bool ReloadAll(const std::string &script_path);

	bool Call(int nargs);
	lua_State * GetL() { return state_; }

	void PushNum(double d);
	void PushUserType(const char *type, void *data);
	void PushString(const char *s);
	void PushLString(const char *s, unsigned int length);

private:
	bool CallPreMain(const std::string &script_path, const std::string &data_dir);
	bool LoadScripts(const std::string &script_path, const std::string &rec_path, bool is_rec = false);

private:
	lua_State *state_;						// 虚拟机
};

#endif // __LUA_MANAGER_HPP__


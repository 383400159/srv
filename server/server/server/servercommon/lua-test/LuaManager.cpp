#include "LuaManager.h"


LuaManager::LuaManager(void)
{
	//打开Lua
	lua_State *L = luaL_newstate();
	/*加载lua所有库*/
	luaL_openlibs(L);
}


LuaManager::~LuaManager(void)
{
}


bool LuaManager::Dofile(char * file_name)
{
	int ret = luaL_dofile(state_, file_name);
	if (ret)                       // 调用出错  
	{
		const char *pErrorMsg = lua_tostring(state_, -1);
		cout << pErrorMsg << endl;
		lua_close(state_);
		return false;
	}
	return true;
}

bool LuaManager::Getglobal(char * func_name)
{
	lua_getglobal(state_, "add_path_to_env");        // 获取函数，压入栈中  
	lua_pushstring(state_, "../global_script/src/db_param/?.lua;");          // 压入参数  
	int Ret = lua_pcall(state_, 1, 1, 0);// 调用函数，调用完成以后，会将返回值压入栈中，第一个1表示参数个数，第二个1表示返回结果个数。
	if (Ret)                       // 调用出错  
	{
		const char *pErrorMsg = lua_tostring(state_, -1);
		cout << pErrorMsg << endl;
		lua_close(state_);
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
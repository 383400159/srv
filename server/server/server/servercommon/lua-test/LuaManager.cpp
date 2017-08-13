#include "LuaManager.h"


LuaManager::LuaManager(void)
{
	//��Lua
	lua_State *L = luaL_newstate();
	/*����lua���п�*/
	luaL_openlibs(L);
}


LuaManager::~LuaManager(void)
{
}


bool LuaManager::Dofile(char * file_name)
{
	int ret = luaL_dofile(state_, file_name);
	if (ret)                       // ���ó���  
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
	lua_getglobal(state_, "add_path_to_env");        // ��ȡ������ѹ��ջ��  
	lua_pushstring(state_, "../global_script/src/db_param/?.lua;");          // ѹ�����  
	int Ret = lua_pcall(state_, 1, 1, 0);// ���ú�������������Ժ󣬻Ὣ����ֵѹ��ջ�У���һ��1��ʾ�����������ڶ���1��ʾ���ؽ��������
	if (Ret)                       // ���ó���  
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
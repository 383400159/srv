#ifndef __LUA_MANAGER_H__
#define __LUA_MANAGER_H__
#include <iostream>
#include "tolua++.h"
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


using namespace std;
static const int SCRIPT_MAX_LARG_NUM = 8;						// 最多往脚本传递8个参数
class LuaManager
{
public:
	LuaManager(void);
	~LuaManager(void);
	
	bool Call(int nargs);
	lua_State * get_L() { return state_; }
	bool Dofile(char * file_name);
	bool Getglobal(char * func_name);

	void PushNum(double d);
	void PushUserType(const char *type, void *data);
	void PushString(const char *s);
	void PushLString(const char *s, unsigned int length);
private:
	lua_State *state_; 
};

#endif // __LUA_MANAGER_H__
#include "ScriptManager.h"


ScriptManager::ScriptManager(void)
{
}


ScriptManager::~ScriptManager(void)
{
}



int ScriptManager::PCall(const char *func, ...)
{
	int top = lua_gettop(lua_mgr_.get_L());

	lua_getglobal(lua_mgr_.get_L(), func);

	int nargs = 0;

	va_list arg_ptr;
	va_start(arg_ptr, func);
	int type = va_arg(arg_ptr, int);

	while (type != LARG_END && nargs < SCRIPT_MAX_LARG_NUM)
	{
		switch (type)
		{
		case LARG_INT32:
			{
				int i = va_arg(arg_ptr, int);
				lua_mgr_.PushNum((double)i);
			}
			break;

		case LARG_UINT32:
			{
				unsigned int ui = va_arg(arg_ptr, unsigned int);
				lua_mgr_.PushNum((double)ui);
			}
			break;

		case LARG_DOUBLE:
			{
				double d = va_arg(arg_ptr, double);
				lua_mgr_.PushNum(d);
			}
			break;

		case LARG_UINT64:
			{
				unsigned long long ulld = va_arg(arg_ptr, unsigned long long);
				double d = *((double *)&ulld);
				lua_mgr_.PushNum(d);
			}
			break;

		case LARG_STRING:
			{
				const char *s = va_arg(arg_ptr, const char *);
				lua_mgr_.PushString(s);
			}
			break;
		case LARG_LSTRING:
			{
				const char *s = va_arg(arg_ptr, const char *);
				unsigned int ui = va_arg(arg_ptr, unsigned int);
				lua_mgr_.PushLString(s, ui);
			}
			break;
		case LARG_USER_TYPE:
			{
				const char *name = va_arg(arg_ptr, const char *);
				void *value = va_arg(arg_ptr, void *);
				lua_mgr_.PushUserType(name, value);
			}
			break;
		}

		++ nargs; type = va_arg(arg_ptr, int);
	}
	va_end(arg_ptr);

	if (nargs >= SCRIPT_MAX_LARG_NUM) {
		lua_settop(lua_mgr_.get_L(), top);
		return false;
	}

	if (!lua_mgr_.Call(nargs)){
		lua_settop(lua_mgr_.get_L(), top);
		return false;
	}

	lua_settop(lua_mgr_.get_L(), top);
	return true;
}
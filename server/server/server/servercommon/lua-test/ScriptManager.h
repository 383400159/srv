#ifndef __SCRIPT_MANAGER_H__
#define __SCRIPT_MANAGER_H__
#include <string>

#include "LuaManager.h"
#include "singleton.h"
#include <cstdint>

class ScriptManager: public Singleton<ScriptManager>
{
public:
	lua_State * GetL() { return lua_mgr_.get_L(); }

private:
	ScriptManager(void);
	~ScriptManager(void);

	int PCall(const char *func, ...);
	LuaManager lua_mgr_;
};
#define sScriptManager ScriptManager::get_instance()
#endif // __SCRIPT_MANAGER_HPP__
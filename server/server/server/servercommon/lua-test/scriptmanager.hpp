#ifndef __SCRIPT_MANAGER_HPP__
#define __SCRIPT_MANAGER_HPP__

#include <string>

#include "luamanager.hpp"
#include "singleton.h"
#include <cstdint>

class ScriptManager : public Singleton<ScriptManager>
{
friend class Singleton<ScriptManager>;
public:
	bool Init(const std::string &lua_dir, const std::string &data_dir, std::string *err);
	bool StartScriptService();
	
	int PCall(const char *func, ...);

	bool ReloadScriptModule(const char *module_name);
	bool ReloadAllScripts();

	int StartGC();
	int TryGC();
	int StopGC();

	void DecStepMul();
	void IncStepMul();

	void MemMonitor();

	lua_State * GetL() { return lua_mgr_.GetL(); }
private:
	ScriptManager();
	~ScriptManager();

	LuaManager lua_mgr_;
	uint32_t step_mul_;
	std::string lua_dir_;
};

#define sScriptManager ScriptManager::get_instance()

#endif // __SCRIPT_MANAGER_HPP__


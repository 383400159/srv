#include "scriptmanager.hpp"
#include "server_time_mgr.hpp"
#include <iostream>
#include <sstream>
ScriptManager::ScriptManager():step_mul_(0)
{

}

ScriptManager::~ScriptManager()
{

}

bool ScriptManager::Init(const std::string &lua_dir, const std::string &data_dir, std::string *err)
{
	char errinfo[1024];

	lua_State *L = lua_open();

	if (!lua_mgr_.Init(L, lua_dir.c_str(), data_dir.c_str()))
	{
		sprintf(errinfo,"%s: Load Scripts Error.", (lua_dir).c_str());
		*err = errinfo;
		return false;
	}

	lua_dir_ = lua_dir;

	StartGC();

	return true;
}

bool ScriptManager::StartScriptService()
{
	if (!lua_mgr_.CallMain(lua_dir_.c_str()))
	{
		return false;
	}

	return true;
}

int ScriptManager::PCall(const char *func, ...)
{
	int top = lua_gettop(lua_mgr_.GetL());

	lua_getglobal(lua_mgr_.GetL(), func);

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
		lua_settop(lua_mgr_.GetL(), top);
		return false;
	}

	if (!lua_mgr_.Call(nargs)){
		lua_settop(lua_mgr_.GetL(), top);
		return false;
	}

	lua_settop(lua_mgr_.GetL(), top);
	return true;
}

bool ScriptManager::ReloadScriptModule(const char *module_name){
	if (0 == PCall("require_ex", LARG_STRING, module_name, LARG_END))
		return false;
	return true;
}

bool ScriptManager::ReloadAllScripts()
{
	return lua_mgr_.ReloadAll(lua_dir_);
}

int ScriptManager::StartGC(){
	lua_gc(lua_mgr_.GetL(), LUA_GCRESTART, 0);

	// 自适应不同的机器效率，设置内存回收的速率
	do{
		step_mul_ += 500;
		lua_gc(lua_mgr_.GetL(), LUA_GCSETSTEPMUL, step_mul_);

		ServerTimeMgr::get_instance().Update();
		uint64_t cur_time = ServerTimeMgr::get_instance().get_millisecond_time();

		if (lua_gc(lua_mgr_.GetL(), LUA_GCSTEP, 1) == 1) {
			lua_gc(lua_mgr_.GetL(), LUA_GCRESTART, 0);
		}

		ServerTimeMgr::get_instance().Update();
		uint64_t escape_time = ServerTimeMgr::get_instance().get_millisecond_time() - cur_time;
		
		if (step_mul_ >= 100000)
			break;

		// 超时尝试(不能超过10毫秒)
		if (escape_time >= 10){
			break;
		}
	}while(true);

	lua_gc(lua_mgr_.GetL(), LUA_GCSETSTEPMUL, step_mul_);
	std::cout << "[SCRIPT] set lua gcstepmul:" << step_mul_ << std::endl; 
	//LOG_MAIN(LL_INFO, "set lua gcstepmul:" << step_mul_);

	// 重启一下gc
	lua_gc(lua_mgr_.GetL(), LUA_GCRESTART, 0);
	
	return 0;
}

int ScriptManager::TryGC(){
	static uint32_t record_count = 0;

	ServerTimeMgr::get_instance().Update();
	uint64_t cur_time = ServerTimeMgr::get_instance().get_millisecond_time();

	lua_gc(lua_mgr_.GetL(), LUA_GCSTEP, 1);
	
	ServerTimeMgr::get_instance().Update();
	long long escape = ServerTimeMgr::get_instance().get_millisecond_time() - cur_time;
	if (escape >= 10){
		DecStepMul();
		record_count = 0;
		return 0;
	}

	if (escape < 8){
		++record_count;
	} else {
		record_count = 0;
	}

	if (record_count >= 5000){
		record_count = 0;
		IncStepMul();
	}
	
	return 0;
}

int ScriptManager::StopGC(){
	lua_gc(lua_mgr_.GetL(), LUA_GCSTOP, 0);
	return 0;
}

void ScriptManager::DecStepMul(){
	step_mul_ = step_mul_ / 2;
	step_mul_ = step_mul_ == 0 ? 1 : step_mul_;
	lua_gc(lua_mgr_.GetL(), LUA_GCRESTART, 0);
	lua_gc(lua_mgr_.GetL(), LUA_GCSETSTEPMUL, step_mul_);
	std::cout << "[SCRIPT] reset lua gcstepmul:" << step_mul_ << std::endl; 
	//LOG_MAIN(LL_INFO, "reset lua gcstepmul:" << step_mul_);
}


void ScriptManager::IncStepMul(){
	if (step_mul_ >= 100000)
		return;

	step_mul_ *= 2;
	if (step_mul_ >= 100000)
		step_mul_ = 100000;

	lua_gc(lua_mgr_.GetL(), LUA_GCRESTART, 0);
	lua_gc(lua_mgr_.GetL(), LUA_GCSETSTEPMUL, step_mul_);
	std::cout << "[SCRIPT] reset lua gcstepmul:" << step_mul_ << std::endl; 
	//LOG_MAIN(LL_INFO, "reset lua gcstepmul:" << step_mul_);
}

void ScriptManager::MemMonitor(){
	lua_gc(GetL(), LUA_GCCOLLECT, 0);

	std::ostringstream oss;
	oss << "Lua Obj Monitor!!!\n" << "[lua mem size:" << lua_gc(lua_mgr_.GetL(), LUA_GCCOUNT, 0) << "] ";
	oss << "\n==========================================================";
	//LOG_MAIN(LL_INFO, oss.str().c_str());
	std::cout << "[SCRIPT] " << oss.str().c_str() << std::endl; 
	sScriptManager.PCall("mem_monitor", LARG_END);
}
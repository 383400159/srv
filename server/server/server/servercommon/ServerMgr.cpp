#include "ServerMgr.h"
#include <utility>

ServerMgr::ServerMgr(void)
{
}


ServerMgr::~ServerMgr(void)
{
}


bool ServerMgr::RegisterModule(const char* name, IModule* module)
{
	m_module_map.insert(std::make_pair(name , module));
	module->set_interface_mgr(this);
	return true;
}
IModule* ServerMgr::QueryModule(const char* name)
{
	auto it = m_module_map.find(name);
	if (it!=m_module_map.end())
	{
		return it->second;
	}
	return nullptr;
}
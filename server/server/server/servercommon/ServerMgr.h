#ifndef SERVERMGR_H
#define SERVERMGR_H

#include "IInterfaceMgr.h"
#include "IModule.h"
#include <list>
#include <map>


class ServerMgr : public IInterfaceMgr
{
	//¼ÇÂ¼·þÎñÆ÷×´Ì¬
	enum ServerState
	{
		GS_STOP = 0,
		GS_RUNNING,
		GS_STOPPING,
	};
	typedef std::map<const char* , IModule*> ModuleMap;
	typedef std::list<IModule*> ModuleList;
public:
	ServerMgr(void);
	~ServerMgr(void);

	bool RegisterModule(const char* name, IModule* module);
	IModule* QueryModule(const char* name);

	ModuleMap m_module_map;

};

#endif
#ifndef IMODULE_H
#define IMODULE_H
#include "IInterfaceMgr.h"

class IModule
{
public:
	IModule(void){};
	~IModule(void){};

	virtual int Init()=0;
	virtual int Start()=0;
	virtual int Update()=0;
	virtual int Stop()=0;
	virtual int Release()=0;

	/* 此函数释放自己所占用的空间，调用此函数后，Game将不再使用此模块的指针 */
	virtual void Free()=0;
	IInterfaceMgr* Interface(){ return m_interface_mgr; }
private:
	IInterfaceMgr* m_interface_mgr;

};

#endif
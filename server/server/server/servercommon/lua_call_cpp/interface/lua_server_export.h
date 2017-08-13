#ifndef __LOTTERY_EXPORT_H__
#define __LOTTERY_EXPORT_H__
#include <iostream>

//tolua_begin
namespace CppSrv{
	inline void DebugLog(std::string log){
		std::cout<<log.c_str()<<std::endl;
	}
}
//tolua_end

#endif
#ifndef __LOTTERY_EXPORT_H__
#define __LOTTERY_EXPORT_H__
#include<iostream>
//tolua_begin
namespace LotteryBox{
	inline void LotteryBoxCallBack(){
		std::cout<<"LotteryBoxCallBack--"<<std::endl;
	}
}
//tolua_end

#endif
#ifndef _ERR_CODE_H_ 
#define _ERR_CODE_H_
	
const static int ERR_OK                        	=   0;	//无错误
const static int ERR_NOT_ENOUHT                	=   1;	//@1 不足
const static int ERR_BAG_TYPE_WRONG            	=   2;	//背包类型错误
const static int ERR_BAG_FULL                  	=   3;	//背包已满
const static int ERR_SET_NAME_NOT_ENOUGH       	=   4;	//改名次数不足
const static int ERR_NOT_ENOUHT_TXT            	=   5;	//@1 不足
const static int ERR_NO_SKILL_ID               	=   6;	//技能没有这个ID
const static int ERR_SKILL_UNLOCK              	=   7;	//技能已经解锁
const static int ERR_NO_SKILL_UNLOCK           	=   8;	//技能未解锁
const static int ERR_NO_SET_FOUR_SKILL         	=   9;	//4号位不让更换
const static int ERR_SET_SKILL                 	=  10;	//技能已经装配
const static int ERR_NO_SET_SKILL              	=  11;	//技能未装配
const static int ERR_SKILL_INTERVAL            	=  12;	//技能区间不对
const static int ERR_NOT                       	=  13;	//@1 
const static int ERR_SET_GENDER_NOT_ENOUGH     	=  14;	//性别修改次数不足
const static int ERR_BATTERY_RATE_ENOUGH       	=  15;	//炮倍数不足
const static int ERR_BATTERY_ITEM_ENOUGH       	=  16;	//解锁材料不足
const static int ERR_GAILY_ASS_ENOUGH          	=  17;	//救济金领取条件不足
const static int ERR_GAILY_ASS_COUNT_ENOUGH    	=  18;	//救济金领取次数不足
const static int ERR_LOTTERY_FREE_RNOUGH       	=  19;	//抽奖免费时间不足
const static int ERR_LOTTERY_TYPE              	=  20;	//抽奖类型不对
const static int ERR_COUNT_ENOUGH              	=  21;	//次数不足
const static int ERR_ISLAND_1                  	=  22;	//进入岛屿金币不足
const static int ERR_ISLAND_2                  	=  23;	//解锁的炮倍等级不足
const static int ERR_ISLAND_3                  	=  24;	//时间条件不足
const static int ERR_ISLAND_4                  	=  25;	//消耗物品不足
const static int ERR_ISLAND_5                  	=  26;	//金币消耗不足
const static int ERR_SIGN_1                    	=  27;	//签到异常 导致条件不足
const static int ERR_SIGN_2                    	=  28;	//今日已经签到 导致条件不足
const static int ERR_SIGN_3                    	=  29;	//签到异常  导致条件不足
const static int ERR_SIGN_4                    	=  30;	//领取条件不足
const static int ERR_SIGN_5                    	=  31;	//物品已经领取 条件不足
const static int ERR_SIGN_6                    	=  32;	//领取时间条件不足
const static int ERR_BUY_1                     	=  33;	//货币类型或者数值验证出错
const static int ERR_TASK_1                    	=  34;	//任务条件不足
const static int ERR_TASK_2                    	=  35;	//任务领取条件不足
const static int ERR_TASK_3                    	=  36;	//成就领取条件不足
const static int ERR_TASK_4                    	=  37;	//成就领取积分条件不足
const static int ERR_COM                       	=  38;	//通讯异常请稍后再试
const static int NOTICE_KILL                   	=   1;	//恭喜[00fffc]@1 [-]在[f36aff]@2 [-]中击杀了[2ea9ff]@3 [-]获得了[ffe400]@4 金币[-]！
const static int NOTICE_BATTERY_RATE           	=   2;	//恭喜@1 的炮台解锁到@2 炮倍，战力更上一层楼！
const static int NOTICE_ALL                    	=   3;	//@1 
		
#endif // _ERR_CODE_H_ 
		
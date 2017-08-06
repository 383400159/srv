
C2S_REQ_TRANSFER_SCENE             	=    2
--gm
C2S_REQ_GM_CMD                     	=    10

GL2GA_KICK_ACCOUNT 					= 1009 		--剔除玩家


GL2C_REP_ROLE_ENTER_GAME			= 20020      -- 玩家请求进入游戏，gl服响应
GL2C_REP_ROLE_INFO_UPDATE 			= 20051
GL2C_CODE_NOTICE		 			= 20052		 -- 提示公告
-------------------------------------------------------------------------------------
--发送到登录(注：这里面的要和login.h里面的协议枚举一一对应)
GL2LS_REQ_CLIENT_LOGOUT				= 2022

-------------------------------------------------------------------------------------

-------------------------------------------------------------------------------------
--发送到聊天服(注：这里面的要和chat.h里面的协议枚举一一对应)
GL2CS_CREATE_USER 					= 4001		--创建一个聊天用户
GL2CS_DESTROY_USER 					= 4002		--销毁一个聊天用户
GL2CS_CHANGE_USER_INFO				= 4003		--用户信息改变同步
GL2CS_CREATE_CHANNEL				= 4004		--创建一个聊天频道
GL2CS_DESTROY_CHANNEL				= 4005		--销毁一个聊天频道
GL2CS_ADD_USER_TO_CHANNEL			= 4006		--用户加入聊天频道
GL2CS_DELETE_USER_FROM_CHANNEL		= 4007		--用户离开聊天频道

-------------------------------------------------------------------------------------

-------------------------------------------------------------------------------------
--发送到gamesrv(注：这里面的要和gamesrv.h里面的协议枚举一一对应)
GL2GW_CREATE_SCENE					= 6050
GL2GW_DESTORY_SCENE					= 6051
GL2GW_ROLE_ENTER_SCENE				= 6053
GL2GW_ROLE_TRANSFER_SCENE			= 6054
GL2GW_KICK_ROLE						= 6055
GL2GW_START_REFRESH					= 6057
	
GL2GW_ROLE_AVATAR_CHANGE			= 6060
GL2GW_ROLE_ATTR_CHANGE				= 6061
GL2GW_ROLE_USE_ITEM					= 6063
GL2GW_ROLE_REVIVE					= 6064
GL2GW_RELOAD_SCRIPT 				= 6070
GL2GW_KILL_FISH		 				= 6071
GL2GW_SYN_ROLE_DATA	 				= 6072
GL2GW_UPDATE_ROLE_DATA	 			= 6073
GL2GW_RARE_UPDATE_ROLE_DATA	 		= 6074
GL2GW_USE_SKILL	 					= 6075
GL2GW_SET_GUN_LV	 				= 6076
GL2GW_SET_SCENE_AI	 				= 6077

-------------------------------------------------------------------------------------

-------------------------------------------------------------------------------------
--发送到httpsrv(注：这里面的要和httpsrv.h里面的协议枚举一一对应)
CONST_GLREP_RESULT					= 7001

-------------------------------------------------------------------------------------

-------------------------------------------------------------------------------------
--发送到center(注：这里面的要和center.h里面的协议枚举一一对应)
GL2CENTER_LOG						= 8001

--进入center的lua协议
GL2CENTER_CDKEY_GIFTBAG				= 8201	--兑换码

--center返回
CENTER2GL_CDKEY_GIFTBAG				= 8501

--单服gl到跨服gl 9000-9249
GL2CROSS_REGISTER					= 9000
GL2CROSS_DESTROY					= 9001
GL2CROSS_PVP_MATCHING				= 9002
GL2CROSS_PVP_CANCEL_MATCHING		= 9003
GL2CROSS_THREEPK_SELF_MATCHING		= 9004
GL2CROSS_THREEPK_TEAM_MATCHING		= 9005
GL2CROSS_THREEPK_CANCEL_SELF_MATCH	= 9006
GL2CROSS_THREEPK_CANCEL_TEAM_MATCH	= 9007

--跨服gl到单服gl 9250-9500
CROSS2GL_REGISTER					= 9250
CROSS2GL_SET_SCENE					= 9251
CROSS2GL_PVP_MATCHING_RESULT		= 9252
CROSS2GL_PVP_FIGHT_RESULT			= 9253
CROSS2GL_THREEPK_MATCHING			= 9254
CROSS2GL_THREEPK_MATCHING_RESULT	= 9255
CROSS2GL_THREEPK_FIGHT_RESULT		= 9256
CROSS2GL_RANK_UPDATE				= 9257		--排行榜变更

-------------------------------------------------------------------------------------

---------------------------------------------------------------------------------------------------------
----------------------------------------华丽的分割线-----------------------------------------------------
---------------------------------------------------------------------------------------------------------




--18000
--------------------------------------------------------------------------------------
--内部句柄消息
ITN_ROLE_ENTER_GAME_FROM_GA			= 1
ITN_LOGOUT_FROM_GA					= 2
ITN_LOGOUT_FROM_GW					= 3
	
ITN_TRANSFER_SCENE					= 20
ITN_SYN_ROLE_INFO_DATA				= 21
ITN_REQ_DESTORY_SCENE				= 22
ITN_SYN_ROLE_POS					= 23
ITN_SYN_ROLE_SCENE_INFO				= 24
	
GW2GL_OUTPUT_CURRENCY 				= 100		--货币产出/消耗
GW2GL_KICK_ALL_LEAVE_SCENE			= 107		--让所有玩家离开当前场景
GW2GL_ATTACK_FISH					= 108		--击中鱼
GW2GL_ADD_GOLD_FISH					= 109		--宝箱类货币增减
GW2GL_CHANGE_GUN					= 110		--换炮
GW2GL_SET_GUN_LV					= 111		--设置最大炮倍
GW2GL_ADD_GOLD_POOL					= 112 		--单独增加
GW2GL_ATTACK_BOSS					= 113 		--boss的爆金
GW2GL_ADD_ITEMS						= 114 		--获得物品
GW2GL_START_BOX_GAME				= 115 		--开启小游戏
GW2GL_START_LOTTERY					= 116 		--开启抽奖

--------------------------------------------------------------------------------------

--------------------------------------------------------------------------------------
--内部内部发送消息

--------------------------------------------------------------------------------------


module( "msg", package.seeall )
dofile_ex( "../global_script/src/protocol/msg_ex" )

-------------------------------------------------------------------------------------
--客户端句柄消息 begin:15000

--room
C2S_ROOM_ENTER_ROOM                	=    50
C2S_ROOM_START_REFRESH             	=    51
C2S_ROOM_EXIT_ROOM                 	=    52
C2S_ROOM_JJJ_KKK                   	=    53
C2S_ATTR_JJJ_KKK                   	=   100

-------------------------------------------------------------------------------------
--返回客户端的消息(注：这里面的要和client.h里面的协议枚举一一对应)
				
--room
GL2C_ROOM_ENTER_ROOM               	= 25050

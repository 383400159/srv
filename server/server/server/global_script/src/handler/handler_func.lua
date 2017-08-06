require "protocol/msg"
require "handler/handler_gm"
require "handler/handler_room"
require "handler/handler_attr"
		
-- 客户端外部消息
game_client_func_table =
{
	[ msg.C2S_REQ_GM_CMD ]                  	= handler.on_gm_cmd,
	--room
	[ msg.C2S_ROOM_ENTER_ROOM ]                	= handler.on_room_enter_room,
	[ msg.C2S_ROOM_START_REFRESH ]             	= handler.on_room_start_refresh,
	[ msg.C2S_ROOM_EXIT_ROOM ]                 	= handler.on_room_exit_room,
	[ msg.C2S_ROOM_JJJ_KKK ]                   	= handler.on_room_jjj_kkk,
	[ msg.C2S_ATTR_JJJ_KKK ]                   	= handler.on_attr_jjj_kkk,
}

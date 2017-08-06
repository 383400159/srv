module( "handler_mgr", package.seeall )

require "handler/handler_func"
require "handler/handler_scene"
require "handler/handler_gw2gl"
local LLOG = LLOG
local LERR = LERR


local PT_DISCONNECT                     = 0x0001
local PT_CONNECT                        = 0x0002

CHECK_PACKET                            = true

proto_handle_time                       = proto_handle_time or 0
proto_handle_rec                        = proto_handle_rec or {}

-- 服务器内部消息
game_internal_func_table =
{
	[ msg.ITN_ROLE_ENTER_GAME_FROM_GA ]             = handler.on_role_enter_game,
	[ msg.ITN_LOGOUT_FROM_GA ]                      = handler.on_logout_from_ga,
	[ msg.ITN_LOGOUT_FROM_GW ]                      = handler.on_logout_from_gw,

	[ msg.ITN_TRANSFER_SCENE ]                      = handler.on_req_transfer_scene,
	[ msg.ITN_SYN_ROLE_INFO_DATA ]                  = handler.on_req_syn_role_info_data,
	[ msg.ITN_REQ_DESTORY_SCENE ]                   = handler.on_req_destory_scene,
	[ msg.ITN_SYN_ROLE_POS ]                   		= handler.on_req_syn_role_pos,
	[ msg.ITN_SYN_ROLE_SCENE_INFO ]                 = handler.on_req_syn_role_scene_info,

	[ msg.GW2GL_OUTPUT_CURRENCY ]                   = handler.on_output_currency,
	[ msg.GW2GL_KICK_ALL_LEAVE_SCENE ]				= handler.on_kick_all_leave_scene,
	[ msg.GW2GL_ATTACK_FISH ]						= handler.on_attack_fish,

	[ msg.GW2GL_ADD_GOLD_FISH ]						= handler.on_add_gold_fish,
	[ msg.GW2GL_CHANGE_GUN]							= handler.on_change_gun,
	[ msg.GW2GL_SET_GUN_LV]							= handler.on_set_gun_lv,

	[ msg.GW2GL_ADD_GOLD_POOL]						= handler.on_add_gold_pool,
	[ msg.GW2GL_ATTACK_BOSS]						= handler.on_attach_boss,

	[ msg.GW2GL_ADD_ITEMS]							= handler.on_add_items,
	[ msg.GW2GL_START_BOX_GAME]						= handler.on_start_box_game,
	[ msg.GW2GL_START_LOTTERY]						= handler.on_start_lottery,
}

function message_client_handler(gsnetid, msg_code, pb_data, length)
	local func = game_client_func_table[msg_code]
	if not func then
		LLOGERROR("[message_client_handler] msg no handler : ".. msg_code)
		return
	end
	
	local role_tbl_gsnetid  = role_mgr.table_role_by_gsnetid
	local role = role_tbl_gsnetid[gsnetid]
	if nil == role then
		LLOGERROR("[message_client_handler] can't found role gsnetid:".. gsnetid)
		return
	end
	
	func( role, pb_data, length )
end

function message_internal_handler(netid, msg_code, pb_data, length)
	local func = game_internal_func_table[msg_code]
	if not func then
		LLOGERROR("[message_internal_handler] msg no handler : ".. msg_code)
		return
	end
	
	func( netid, pb_data, length )
end


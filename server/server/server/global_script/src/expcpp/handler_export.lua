require "handler_mgr"
require "sys_cmd_mgr"
require "role_mgr"
require "rank_mgr"
require "object/role_t"

require "protocol/NP_CS_Gm_pb"

local handler_mgr = handler_mgr
local NP_CS_Gm_pb = NP_CS_Gm_pb
local sys_cmd_mgr = sys_cmd_mgr
local role_mgr = role_mgr
local role_t = role_t
local rank_mgr = rank_mgr

---------------------------------------------------------------------------------------------------------
----------------------------------------华丽的分割线-----------------------------------------------------
---------------------------------------------------------------------------------------------------------

function handle_client_msg(gsnetid, msg_code, pb_data, length)
	handler_mgr.message_client_handler(gsnetid, msg_code, pb_data, length)
end

-- 由c++函数GA2GLReqClientEnterGame调用此lua函数
function handle_internal_msg(netid, msg_code, pb_data, length)
	handler_mgr.message_internal_handler(netid, msg_code, pb_data, length)
end

function handle_console_msg(pb_data, length)
	local msg = NP_CS_Gm_pb.C2SReqGmCmd()
	msg:ParseFromString(pb_data)

	sys_cmd_mgr.cmd_call_by_dos(msg.cmd)
end

function handle_sys_cmd_msg(idx,cmd)
	sys_cmd_mgr.cmd_call(idx, cmd)
end

function handle_platform_callback(is_suc, gsnetid, str_data)
	local login_state = role_mgr.get_role_by_gsnetid(gsnetid)

	if not login_state then return end
	if 1 ~= login_state then
		LLOGERROR ( "login error!!!gsnetid:" ..
			gsnetid .. ",login state:"..#login_state)
		return
	end

	if 1 ~= is_suc then
		LLOGERROR ( "login error2!!!gsnetid:" .. gsnetid .. ",login state:" .. login_state)
		return
	end

	local tData = util.unserialize(str_data)
	if type(tData) ~= "table" then
		LLOGERROR ( "login error type(tData)~=table !!! gsnetid:" .. gsnetid .. ", login state:" .. login_state .. ", is_suc：" .. is_suc .. 
			", str_data: " .. str_data)
		return
	end

	-- for k,v in pairs(tData) doF
	-- 	LLOGINFO("handle_platform_callback:"..k.." "..v)
	-- end

	local role = role_t.new()
	role.attr.gsnetid = gsnetid
	role.attr.role_id = tData.role_id
	role.attr.user_name = tData.user_name
	--role.attr.gold = tData.gold
	--role.attr.diamond = tData.diamond
	--role.attr.vip_level = tData.vip_level
	role.attr.name     = tData.name
	role.attr.token = tData.token
	role.attr.gameid = tData.gameid

	role_mgr.add_role(role)

    -- 从平台库中获取玩家捕鱼数据
    --LLOGINFO("handle_platform_callback")
    -- CppSrv.get_player_data(""..role.attr.role_id)
    
end

-- 获取玩家捕鱼数据回调
function get_player_data_callback(is_suc, key_id, str_db_data)
	if 1 ~= is_suc then
		LLOGERROR ( "get_player_data_callback fail key_id: " .. key_id)
		return
	end

	-- --派分全局数据
	-- local t = {
	-- 			rankMgrTable = rank_mgr.load_db_rankMgrTable,
	-- 			rankResTable = rank_mgr.load_db_rankResTable,
	-- 			rankRetTable = rank_mgr.load_db_rankRetTable,
	-- 			cmdCallByDosReload = sys_cmd_mgr.load_db_cmdCallByDosReload,
	-- 			setSceneAI	= scene_mgr.load_db_setSceneAI,
	-- 			}
	-- if t[key_id]~=nil then
	-- 	local func = t[key_id]
	-- 	func(str_db_data)
	-- else
	-- 	--转化为int 为role_id
	-- 	key_id = tonumber(key_id)
	-- 	local role = role_mgr.get_role_by_id( key_id )
	-- 	if not role then
	-- 		LLOGERROR ( "get_player_data_callback can not find role!!! role_id:" .. key_id)
	-- 		return
	--     end

	-- 	role:init_from_db(str_db_data, key_id)
	-- 	role:init_login()
	-- 	role:enter_game()
	-- end	


end




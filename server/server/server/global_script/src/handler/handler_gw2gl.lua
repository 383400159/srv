module( "handler", package.seeall )

require "protocol/msg"
require "role_mgr"
require "res_mgr"
require "scene_mgr"
require "protocol/gw2gl_pb"


local mssg				= msg
local role_mgr 			= role_mgr
local scene_mgr 		= scene_mgr
local gw2gl_pb 			= gw2gl_pb
local code_mgr			= code_mgr
local code_define		= code_define
local pow 				= math.pow
local random            = math.random
local floor             = math.floor
local Tonumber 			= tonumber
local format			= string.format
---------------------------------------------------------------------------------------------------------
----------------------------------------华丽的分割线-----------------------------------------------------
---------------------------------------------------------------------------------------------------------

function on_req_transfer_scene(netid, pb_data, length)
	local msg = gw2gl_pb.GW2GLTransferScene()
	msg:ParseFromString(pb_data)

	local role = role_mgr.table_role_id[msg.role_id]
	if nil == role then
		LLOGERROR( "can't found role, role id:" .. msg.role_id)
		return
	end

	scene_mgr.role_transfer_scene(role, msg.scene_type_id, msg.scene_x, msg.scene_y,true)
end

function on_req_destory_scene(netid, pb_data, length)
	local msg = gw2gl_pb.GW2GLReqDestoryScene()
	msg:ParseFromString(pb_data)

	scene_mgr.destory_scene(msg.scene_id)
end

function on_req_syn_role_info_data(netid, pb_data, length)
	local msg = gw2gl_pb.GW2GLSynRoleDataInfo()
	msg:ParseFromString(pb_data)

	local role = role_mgr.table_role_id[msg.role_id]
	if nil == role then
		LLOGERROR( "[on_logout_from_gw] can't found role, role id:" .. msg.role_id)
		return
	end

	role:syn_role_info_data(msg)
end

function on_req_syn_role_pos(netid, pb_data, length)
	local msg = gw2gl_pb.GW2GLSynRolePos()
	msg:ParseFromString(pb_data)

	local role = role_mgr.table_role_id[msg.role_id]
	if nil == role then
		LLOGERROR( "[on_req_syn_role_pos] can't found role, role id:" .. msg.role_id)
		return
	end

	role:syn_role_pos(msg)
end

function on_req_syn_role_scene_info(netid, pb_data, length)
	local msg = gw2gl_pb.GW2GLSynRoleSceneInfo()
	msg:ParseFromString(pb_data)

	local role = role_mgr.table_role_id[msg.role_id]
	if role then
		role:syn_role_scene_info(msg)
	end

end

function on_kick_all_leave_scene(netid,pb_data,length)
	local msg = gw2gl_pb.GW2GLKickAllLeaveScene()
	msg:ParseFromString(pb_data)
	for k,v in ipairs(msg.role_ids) do
		local role = role_mgr.table_role_id[v]
		if role then
			role.misc_control:goback_scene()
			--.role_transfer_scene(role,role.scene_type_id,role.last_scene_x,role.last_scene_y)
		end
	end
end






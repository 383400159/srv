-- @version 1.0
-- @author	yangenliang
-- @date	2016-06-17 14:18:30.
-- @system	chat_mgr

module('chat_mgr',package.seeall )

function enter_chat( role )
	local pack		= gl2cs_pb.GL2CSCreateUser()
	pack.uid		= role.role_id
	pack.netid		= CppNetwork.get_netid(role.attr.gsnetid)
	pack.node_idx	= CppSrv.get_ga_srv_idx_by_gsnetid(role.attr.gsnetid)
	pack.name		= role.name
	CppNetwork.send_to_chat(msg.GL2CS_CREATE_USER, pack:SerializeToString(), pack:ByteSize())
	change_chat_user_info(role)
end

function leave_chat( role )
	local pack = gl2cs_pb.GL2CSDestroyUser()
	pack.uid = role.role_id
	CppNetwork.send_to_chat(msg.GL2CS_DESTROY_USER, pack:SerializeToString(), pack:ByteSize())
end

function change_chat_user_info( role )
	local pack = gl2cs_pb.GL2CSChangeUserInfo()
	pack.uid = role.role_id
	pack.avatar = role.avatar
	pack.step_level = 1
	pack.camp = role.camp
	pack.camp_post = 0
	pack.guild_post = 0
	pack.prof = role.prof
	pack.sex  = role.sex
	pack.vip_type = role.attr_control:get_value(attr_define.CONST_ATTR_FEILD_IDX_VIP_LEVEL)
	pack.authority_type = role.authority_type
	pack.level = role:get_level()
	pack.zhuan = role:get_zhuan()
	pack.forbid_time = role.chat_forbid_time
	pack.name = role.name
	CppNetwork.send_to_chat(msg.GL2CS_CHANGE_USER_INFO, pack:SerializeToString(), pack:ByteSize())
	-- check team channel
	local team = team_mgr.g_role_team_table[role.role_id]
	if team then
        add_user_to_chat_channel(role.role_id, 200000 + team.team_id)
	end
end

function create_chat_channel(cid)
	local pack = gl2cs_pb.GL2CSCreateChannel()
	pack.cid		= cid
	pack.cooldown	= 2
	pack.type		= 4
	pack.name		= tostring(cid)
	CppNetwork.send_to_chat(msg.GL2CS_CREATE_CHANNEL, pack:SerializeToString(), pack:ByteSize())
end

function destroy_chat_channel(cid)
	local pack = gl2cs_pb.GL2CSDestroyChannel()
	pack.cid		= cid
	CppNetwork.send_to_chat(msg.GL2CS_DESTROY_CHANNEL, pack:SerializeToString(), pack:ByteSize())
end

function add_user_to_chat_channel(role_id, cid)
	local pack = gl2cs_pb.GL2CSAddUserToChannel()
	pack.uid		= role_id
	pack.cid		= cid
	CppNetwork.send_to_chat(msg.GL2CS_ADD_USER_TO_CHANNEL, pack:SerializeToString(), pack:ByteSize())
end

function delete_user_from_chat_channel(role_id, cid)
	local pack = gl2cs_pb.GL2CSDeleteUserFromChannel()
	pack.uid		= role_id
	pack.cid		= cid
	CppNetwork.send_to_chat(msg.GL2CS_DELETE_USER_FROM_CHANNEL, pack:SerializeToString(), pack:ByteSize())
end

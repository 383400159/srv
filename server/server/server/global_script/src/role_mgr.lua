print("role_mgr.lua")
module( "role_mgr", package.seeall )
require "sys_cmd_mgr"
require "protocol/gl2ga_pb"
local sys_cmd_mgr = sys_cmd_mgr
table_role_id = table_role_id or {}
table_role_gsnetid = table_role_gsnetid or {}
table_role_by_gsnetid = table_role_by_gsnetid or {}
table_role_user_name = table_role_user_name or {} --实际为user_code
table_logout_table = table_logout_table or {}
g_role_num = g_role_num or 0
--缓存玩家离线邮件
mail_cache_list = mail_cache_list or {}
--缓存快速查看的玩家数据 个人中心数据
personal_list = personal_list or {}
--玩家待发送物品列表
recharged_list = recharged_list or {}
--玩家已发放物品列表
recharged_send_list = recharged_send_list or {}

is_init = true
is_save = false


function add_loging_role(gsnetid)
	table_role_gsnetid[gsnetid] = 1

end

function remove_login_role(gsnetid)
	table_role_gsnetid[gsnetid] = nil
	table_role_by_gsnetid[gsnetid] = nil
end

function add_role( role )
	--假如玩家在线踢掉
	if table_role_user_name[role.attr.user_name]~=nil then
		local old_role = table_role_user_name[role.attr.user_name]
		--踢掉该用户
		local pack = gl2ga_pb.GL2GAKickRole()
		pack.net_id = old_role.attr.gsnetid
		role:send_to_gateway(1009, pack)

		remove_role(old_role)
		old_role.gw_id = 0
		LLOGINFO("重复登录剔除--:"..role.attr.user_name)
	end


	LLOGINFO("add_role:"..role.attr.role_id.." role num:"..(g_role_num+1))
	g_role_num = g_role_num + 1
	table_role_id[role.attr.role_id] = role
	table_role_by_gsnetid[role.attr.gsnetid] = role
	table_role_user_name[role.attr.user_name] =role
	table_logout_table[role.attr.role_id] = nil
	
end


function remove_role( role )
	LLOGINFO("remove_role:"..role.attr.role_id)
	scene_mgr.exit_scene(role)
	g_role_num = g_role_num - 1
	table_role_id[role.attr.role_id] = nil
	table_role_user_name[role.attr.user_name] = nil

end



function get_role_by_id( role_id )
	return table_role_id[role_id]
end

function get_role_by_gsnetid( gsnetid )
	return table_role_gsnetid[gsnetid]
end

function get_role_by_user_name( user_name )
	return table_role_user_name[user_name]
end

function get_all_role()
	return table_role_id
end


-- 一个容错处理
function add_logout_role(role_id)
	if nil == table_logout_table[role_id] then
		table_logout_table[role_id] = 0
	end
end

function update( interval, day_id)
	for _, v in pairs(table_role_id) do
		v:update( interval, day_id)
	end

	for id, time in pairs(table_logout_table) do
		table_logout_table[id] = time + interval
		local role =role_mgr.table_role_id[id]
		if nil == role then
			-- 玩家正常下线了，这里就不用执行了
			table_logout_table[id] = nil
		elseif table_logout_table[id] >= 1000 then
			LLOGDEBUG("table_logout_table >=1000:"..id)
			role:logout_from_gw(1)
			table_logout_table[id] = nil
			g_role_num = g_role_num - 1
			table_role_id[role.attr.role_id] = nil
			table_role_user_name[role.attr.user_name] = nil
		end
	end


	if is_init then
		init()
		is_init = false

	end

end
-----------------------------------------------------------------

function  init()

end

-- 全部保存数据
function do_save_all_role_data()
	for k,v in pairs(table_role_id) do
		v:do_save(0)
	end
end
function dynamics_add_event_to_gw(role,type,params)
	local pack = gl2gw_pb.GL2GWDynamicsAddEvent()
	pack.type = type
	pack.role_id = role.attr.role_id
	for k,v in ipairs(params) do
		pack.params:append(v)
	end
	role:send_game_pack(msg.GL2GW_DYNAMIC_ADD_EVENT, pack)
end










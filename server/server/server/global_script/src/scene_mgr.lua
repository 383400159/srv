-- @version    1.0
-- @author     yangenliang
-- @date       2017-02-24 10:07:33.
-- @system     scene_mgr

require "scene/scene_t"
require "res_mgr"
require "protocol/gl2gw_pb"

local scene_t = scene_t
local gl2gw_pb = gl2gw_pb

module ( "scene_mgr", package.seeall )

g_gamesrv_list = g_gamesrv_list or {} -- 保存game_srv服的id索引
g_gamesrv_num = g_gamesrv_num or 0 -- 已连到当前进程的gamesrv服（进程）的数量
g_scene_map = g_scene_map or {} -- 场景id -> 场景对象
g_gw_id_scene_num_map = g_gw_id_scene_num_map or {} --每台gameworld进程下面的服务器
g_scene_map_by_room_id = g_scene_map_by_room_id or {}
g_current_gw_index	= g_current_gw_index or 1 -- 要分配的gamesrv服对应g_gamesrv_list的数组索引的增长种子
g_instance_inc_num  = g_instance_inc_num or 1 -- 场景id增长种子
g_scene_gold_pool  = g_scene_gold_pool or {}
next_res_time = next_res_time or 0
--g_room_max_role_cnt = 4 -- 房间最大人数
local sin = math.sin
function insert_game(gw_id)
	LLOGINFO ( "scene_mgr  insert_game  gw_id: " .. gw_id)
	table.insert(g_gamesrv_list, gw_id)
	g_gw_id_scene_num_map[""..gw_id] = 0
	g_gamesrv_num = g_gamesrv_num + 1
end

function remove_game( gw_id )
	LLOGDEBUG ( "scene_mgr  remove_game  gw_id: " .. gw_id)
	for k, v in pairs(g_gamesrv_list) do
		if v == gw_id then
			table.remove(g_gamesrv_list, k)
			g_gamesrv_num = g_gamesrv_num - 1
			for m_scene_id, m_scene in pairs(g_scene_map) do
		        if m_scene.gw_id == gw_id then
		        	g_gw_id_scene_num_map[""..gw_id] = nil
			        g_scene_map[m_scene_id] = nil
			        g_scene_map_by_room_id[m_scene.scene_type_id][m_scene_id] = nil
			        LLOGDEBUG ( "scene_mgr  remove_scene  m_scene_id: " .. m_scene_id)
		        end
	        end
			return
		end
	end
end

-- 分配game_world(game_srv)服 id
function get_random_gw()
	if g_gamesrv_num <= 0 then return nil end
	local index = g_current_gw_index % g_gamesrv_num + 1 
	--均衡分配各台game服务器

	--LLOGINFO("index:"..index)
	g_current_gw_index = g_current_gw_index + 1
	return g_gamesrv_list[index]
end

function remove_scene_by_gw_idx( gw_id )

end

------------------------------------------------------------------------------------
-- 场景相关
------------------------------------------------------------------------------------

-- 检查场景
function check_scene()
    if g_gamesrv_num <= 0 then 
    	return 2 
    end

    return 0
end

function get_enter_scene(scene_type_id,room_type)
	local scene_map =  g_scene_map_by_room_id[scene_type_id] or {}
	local scene = nil


	--看看有没有空房 
	--分房规则
	local cache_num = 0
	for k,v in pairs(scene_map) do
		local role_cnt = v:get_role_cnt()
		LLOGINFO("get_enter_scene:"..v.scene_id.." role_cnt:"..role_cnt.." room_max_role_num:"..v.room_max_role_num.." room_type:"..v.room_type)
		if role_cnt < v.room_max_role_num and v.room_type==room_type and (role_cnt>cache_num or v.room_max_role_num==1 or scene==nil) then
			scene = v
			cache_num = role_cnt
			--break
		end
	end

	--无房创建
	if not scene then
		scene = create_scene(scene_type_id,room_type)
	end
	return scene
end

function enter_scene(role, scene_type_id,room_type)
	
	local scene = get_enter_scene(scene_type_id,room_type)
	if not scene then return end
	--LLOGINFO("enter_scene scene_type_id:"..scene_type_id.." room_type:"..room_type.." role_cnt:"..scene.role_cnt)
	--role.scene_id = scene.scene_id
	role.sign_control:enter_scene(scene.scene_id)
	role.attr:enter_scene(scene.scene_id)
	role_enter_scene(role, scene)
	--LLOGINFO("----------------------enter_scene:"..role.attr.scene_id)


end

function leave_scene(role)
	local scene = g_scene_map[role.attr.scene_id]
	if scene then
		scene:del_role()
		role.sign_control:exit_scene(role.attr.scene_id)
		role.attr:exit_scene(role.attr.scene_id)
		LLOGDEBUG("-----------leave_scene ")
	end
end

function exit_scene(role)
	local scene = g_scene_map[role.attr.scene_id]
	if scene then
		local pack = gl2gw_pb.GL2GWKickRole()
		pack.role_id = role.attr.role_id
		pack.type = 1
		role:send_game_pack(msg.GL2GW_KICK_ROLE, pack)
		LLOGDEBUG("----------------------exit_scene:"..role.attr.scene_id)
		LLOGDEBUG("-----------logout_from_ga exit_scene2")
		leave_scene(role)
	else
		LLOGINFO("scene is nil by role_id:"..role.attr.role_id.." "..#g_scene_map)	
	end

end

function get_scene(scene_id)

	local scene = g_scene_map[scene_id]
	return scene
end

function create_scene(scene_type_id,room_type)
	--LLOGINFO("create_scene:"..scene_type_id)
	local gw_id = get_random_gw()
	if not gw_id then
		LLOGERROR("create_scene gw_id==nil scene_type_id: " .. scene_type_id)
		return nil
	end
	local scene_id = g_instance_inc_num % 0xFFff --循环使用
	local scene = scene_t.new(gw_id, scene_id, scene_type_id,room_type)
	g_instance_inc_num = g_instance_inc_num + 1
	g_scene_map[scene_id] = scene
	g_scene_map_by_room_id[scene_type_id] = g_scene_map_by_room_id[scene_type_id] or {}
	g_scene_map_by_room_id[scene_type_id][scene_id] = scene
	if g_gw_id_scene_num_map[""..gw_id]==nil then
		g_gw_id_scene_num_map[""..gw_id] = 0
	end
	g_gw_id_scene_num_map[""..gw_id] = g_gw_id_scene_num_map[""..gw_id] + 1
	local gun_rate_range =  res_mgr.island[scene.scene_type_id].gun_rate_range
	local pack = gl2gw_pb.GL2GWCreateScene()
	pack.scene_id = scene_id
	pack.scene_type_id = scene_type_id	
	pack.max_role_count = scene.room_max_role_num
	pack.room_type = scene.room_type
	pack.min_gun_lv = gun_rate_range[1]
	pack.max_gun_lv = gun_rate_range[2]
	pack.room_rate  = res_mgr.island[scene.scene_type_id].room_rate
	--LLOGINFO("GL2GW_CREATE_SCENE:"..scene_id.." gw_id:"..gw_id)
	CppNetwork.send_to_gamesrv(gw_id, msg.GL2GW_CREATE_SCENE, pack:SerializeToString(), pack:ByteSize())
	return scene
end

function role_enter_scene(role, scene)
	-- local gw_srv_id = scene.gw_id
	-- local gun_rate_range =  res_mgr.island[scene.scene_type_id].gun_rate_range
	-- if gw_srv_id ~= role.gw_id then
	-- 	-- enter scene
	-- 	local pack = gl2gw_pb.GL2GWRoleEnterScene()
	-- 	pack.enter_type = 0
	-- 	pack.scene_id = scene.scene_id
	-- 	pack.ga_srv_idx = CppSrv.get_ga_srv_idx_by_gsnetid(role.attr.gsnetid)
	-- 	pack.gsnetid = role.attr.gsnetid
	-- 	pack.server_id = g_server_id or 1
	-- 	pack.role_id = role.attr.role_id
	-- 	pack.user_code = role.attr.user_name
	-- 	pack.token = role.attr.token
	-- 	pack.gold = role.attr.gold
	-- 	pack.battery_id = role.attr.battery_id
	-- 	pack.vip_level = role.attr.vip_level
	-- 	pack.diamond  = role.attr.diamond
	-- 	pack.name = role.attr.name
	-- 	pack.level = role.attr.level
	-- 	pack.head_id = role.attr.head_id
	-- 	pack.gender = role.attr.gender
	-- 	pack.battery_rate = gun_rate_range[2]>role.attr.battery_rate and role.attr.battery_rate or gun_rate_range[2]
	-- 	pack.min_battery_rate = gun_rate_range[1]
	-- 	pack.send_bullet_num = role.attr.send_bullet_num

	-- 	role.gw_id = gw_srv_id
	-- 	role.scene_type_id = scene.scene_type_id
	-- 	--CppNetwork.send_to_gamesrv(gw_srv_id, msg.GL2GW_ROLE_ENTER_SCENE, pack:SerializeToString(), pack:ByteSize())
	-- 	role:send_game_pack(msg.GL2GW_ROLE_ENTER_SCENE, pack)
	-- 	--LLOGINFO("GL2GW_ROLE_ENTER_SCENE2----------------------")
	-- else
	-- 	-- transferscene
	-- 	local pack = gl2gw_pb.GL2GWRoleTransferScene()
	-- 	pack.role_id	= role.attr.role_id
	-- 	pack.scene_id	= role.attr.scene_id
	-- 	role:send_game_pack(msg.GL2GW_ROLE_TRANSFER_SCENE, pack)
	-- 	LLOGINFO("GL2GW_ROLE_TRANSFER_SCENE2---------------------- gw_srv_id:"..gw_srv_id.." gw_id:"..role.gw_id)
	-- end
	-- scene:add_role()
	-- role.scene_type_id = scene.scene_type_id
	-- role.gw_id = gw_srv_id


end


function GetPreciseDecimal(nNum, n)
    if type(nNum) ~= "number" then
        return nNum;
    end
    
    n = n or 0;
    n = math.floor(n)
    local fmt = '%.' .. n .. 'f'
    local nRet = tonumber(string.format(fmt, nNum))

    return nRet;
end








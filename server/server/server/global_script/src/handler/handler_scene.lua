require "role_mgr"

local role_mgr = role_mgr

g_test_role_id = g_test_role_id or 0

module( "handler", package.seeall )

function on_role_enter_game(gsnetid, pb_data, length)
	-- 记录住这个登录
	if nil ~= role_mgr.get_role_by_gsnetid(gsnetid) then
		-- LLOGERROR ( "enter game repeated!!! gsnetid id:" .. gsnetid )
		print ( "enter game repeated!!! gsnetid id:" .. gsnetid )
		return
	end
	role_mgr.add_loging_role(gsnetid)

    local msg = NP_ClientServer_pb.C2SEnterGame()
 	msg:ParseFromString(pb_data)
	-- 这里调用平台获取数据的接口
	CppSrv.get_player_base_data(gsnetid, msg.init_user_name, msg.user_pwd)
	-- 回调函数为handler_export 中的handle_platform_callback

	-- 测试时直接返回
	if CppSrv.get_is_windows() then
		test_call_back(1, gsnetid)
	end
end

-- 这里是测试函数，不通过平台直接返回成功
function test_call_back(is_suc, gsnetid)
	local login_state = role_mgr.get_role_by_gsnetid(gsnetid)

	if not login_state then return end
	if 1 ~= login_state then
		LLOGERROR ( "login error!!!gsnetid:" ..
			gsnetid .. ",login state:" .. login_state)
		return
	end

	if 1 ~= is_suc then
		LLOGERROR ( "login error2!!!gsnetid:" .. gsnetid .. ",login state:" .. login_state)
		return
	end

	local role = role_t.new()
	role.attr.gsnetid = gsnetid
	role.attr.role_id = g_test_role_id
	role.attr.user_name = "testname" .. g_test_role_id
	--role.attr.gold = 10000
	--role.attr.diamond = 0
	role.attr.vip_level = 0
	role.attr.name     = "测试服" .. g_test_role_id
	role.attr.token	= ""
	g_test_role_id = g_test_role_id + 1

	role_mgr.add_role(role)

	-- 直接返回数据
	role:init_from_db("", role.attr.role_id)
	role:init_login()
	role:enter_game()
end

function on_logout_from_ga(netid, pb_data, length)
	local msg = ga2gl_pb.GA2GLRoleLogout()
	msg:ParseFromString(pb_data)

	local gsnetid = CppNetwork.make_gsnetid(netid, msg.gsnetid)
	local role = role_mgr.table_role_by_gsnetid[gsnetid]

	-- 这个角色连接的时候就断开了,这样的一个处理即使是查询数据库返回，也当做下线了。非常完美
	role_mgr.table_role_gsnetid[gsnetid] = nil
	role_mgr.table_role_by_gsnetid[gsnetid] = nil
	if nil == role then
		-- 可能玩家登陆错误了,也可能是从来没登录进游戏服
		LLOGDEBUG( "can't found role, role gsnetid:" ..gsnetid)
		return
	end
	LLOGDEBUG("on_logout_from_ga-------------------------------------- ")
	-- 这个角色已经登录成功了
	if type(role) == "table" then
		role:logout_from_ga()
		role_mgr.add_logout_role(role.attr.role_id)
	end
end

function on_logout_from_gw(netid, pb_data, length)
	local msg = gw2gl_pb.GW2GLRoleLogout()
	msg:ParseFromString(pb_data)

	local role =role_mgr.table_role_id[msg.role_id]
	if nil == role then
		LLOGERROR( "[on_logout_from_gw] can't found role, role id:" .. msg.role_id)
		return
	end
	LLOGDEBUG("on_logout_from_gw---------------------------------------- type:"..msg.type)
	role:logout_from_gw(msg.type)
end

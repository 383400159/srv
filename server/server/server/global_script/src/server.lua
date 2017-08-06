print "server.lua"
require "role_mgr"
require "rank_mgr"
require "scene_mgr"
require "sys_cmd_mgr"
local role_mgr = role_mgr
local rank_mgr = rank_mgr
local sys_cmd_mgr = sys_cmd_mgr
g_update_interval = 1000   --这个服务器不需要跳那么快,一秒钟跳一次够了
g_current_time = g_current_time or 0
_serverVersion = 1.001 --当前服务器版本号
_runMode = 0 -- 0是debug模式  1是release模式
function start()
end

function update( interval )
	g_current_time = g_current_time + interval
	local dayid = CppSrv.get_day_id()
	if g_current_time >= g_update_interval then
		role_mgr.update(g_current_time, dayid)
		g_current_time = 0
	end
end

function set_server_run_mode(type)
	print("set_server_run_mode ",type)
	_runMode = type
end

function stop()
end

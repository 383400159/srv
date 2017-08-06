require "scene_mgr"

local scene_mgr 	= scene_mgr

---------------------------------------------------------------------------------------------------------
----------------------------------------华丽的分割线-----------------------------------------------------
---------------------------------------------------------------------------------------------------------

function on_gamesrv_register(srv_idx)
	scene_mgr.insert_game(srv_idx)
end

function on_gamesrv_leave(srv_idx)
	scene_mgr.remove_scene_by_gw_idx(srv_idx)
	scene_mgr.remove_game(srv_idx)
end

-- 网关断开需要踢掉该网关连接的用户
function on_gateway_leave(netid)
	print("on_gateway_leave")
end

-- @version    1.0
-- @author     feng
-- @date       time.
-- @system     attr_control

require "protocol/NP_SC_Attr_pb"
require "scene_mgr"
require "rank_mgr"
require "server"
require "func"
local attr_define = attr_define
local NP_SC_Attr_pb = NP_SC_Attr_pb

module("attr_control_t", package.seeall)

function new(role)
	local obj = { role = role }
	role.attr_control = obj
	setmetatable( obj, { __index = attr_control_t} )
	obj:init()
	return obj
end

--初始化
----------------------------------------------------------------------
function init(self)
	self.gold 		= 0		---金币
	self.diamond 	= 0      --钻石
	self.name     	= 0  		---姓名

	


	self.scene_id = 0
	self.role_id = 0
	self.is_need_send = false
	self.client_mod = {}
	self.is_need_client = false
	self.token = 0
	self.user_name = "0"
	self.gsnetid = 0
	self.gameid = 0

end

function init_login(self)
end

function init_first_login(self)

end
--隔天处理
function on_daily_change(self) 
	
end

function update_module(self,interval)
	
end

--存档和读档
----------------------------------------------------------------------
function load_db_data(self, tbl)
	
end

function get_db_data(self, tbl)

end

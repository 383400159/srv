-- @version    1.0
-- @author     yangenliang
-- @date       2017-04-06 09:52:37.
-- @system     role

module("role_t", package.seeall)

require_ex "object/ctl/attr_control_t"
require "scene_mgr"
function new()
	local obj = new_tbl("role_t")
	setmetatable( obj, { __index = role_t } )

	obj:init()
	obj:new_ctls()
	return obj
end

--初始化
----------------------------------------------------------------------
function init(self)
	
	--self.scene_id = 0

	self.last_online_dayid=nil
	self.cannon_multiple= 1
	self.gw_srv_id		= 0
	self.scene_type_id = 0
	self.gw_id 			=nil

	self.last_save_time = 5*1000
	self.is_save = false
	self.is_save_count = 0
	-- 用列表管理模块
	self.control_list	= {}
	self.attr = {}
end


function new_ctls( self )	
	table.insert(self.control_list, attr_control_t.new(self))
	self.attr = self.attr_control

end

function init_login(self)
	for i,v in ipairs(self.control_list) do
		if v.init_login then
			v:init_login()
		end
	end
end

function init_first_login(self)
	for i,v in ipairs(self.control_list) do
		if v.init_first_login then
			v:init_first_login()
		end
	end
end

function on_daily_change(self)     
	for i,v in ipairs(self.control_list) do
		if v.on_daily_change then
			v:on_daily_change()
		end
	end
end

--存档和读档
----------------------------------------------------------------------
function load_db_data(self, tbl)
	-- if not tbl["role_t"] then return end
	-- self.cannon_multiple 	= tbl["role_t"].cannon_multiple
	-- self.gw_srv_id 			= tbl["role_t"].gw_srv_id
	-- self.last_online_dayid  = tbl["role_t"].last_online_dayid
end

function get_db_data(self, tbl)
	-- tbl["role_t"] = {
	-- 	cannon_multiple	= self.cannon_multiple,
	-- 	gw_srv_id		= self.gw_srv_id,
	-- 	last_online_dayid = self.last_online_dayid
	-- }
end


function init_from_db(self, tbl_str, role_id)
	if 0 == #tbl_str then return end

	local tbl = util.unserialize(tbl_str)

	if role_id ~= self.attr.role_id then
		LLOGERROR("gsnetid repeat!!!dismiss this login process!!!")
		return false
	end
	-- 基础
	self:load_db_data(tbl)

	-- 循环获取每个模块的数据
	for idx,control in ipairs(self.control_list) do
		control:load_db_data(tbl)
	end
	return true
end

function update(self, interval, dayid)
	self.last_save_time = self.last_save_time + interval

	for i,v in ipairs(self.control_list) do
		if v.update_module then
			v:update_module(interval)
		end
	end

	if self.last_online_dayid ~= dayid or self.last_online_dayid==nil  then
		self:on_daily_change(self.last_online_dayid, dayid)
		self.last_online_dayid = dayid
	end
end

function save(self, is_logout)
	if is_logout == 1 then
		role_mgr.remove_role( self )
	end
	if self.is_save_count>0 then
		self:do_save()
	end
end


function do_save(self)
	LLOGDEBUG("do_save-------------- :"..self.attr.role_id)
	local tbl = {}
	--基础
	self:get_db_data(tbl)

	-- 循环获取每个模块的数据
	for idx,control in ipairs(self.control_list) do
		control:get_db_data(tbl)
	end
    CppSrv.set_player_data(""..self.attr.role_id, util.serialize(tbl))
end

function enter_game( self )
	--登陆即做隔天处理
	local dayid = CppSrv.get_day_id()
	if self.last_online_dayid ~= dayid or self.last_online_dayid==nil then
		self:on_daily_change(self.last_online_dayid, dayid)
		self.last_online_dayid = dayid
	end

	return true
end


function logout_from_ga( self )
	if self.attr.scene_id~=nil and self.attr.scene_id>0 then
		local pack = gl2gw_pb.GL2GWKickRole()
		pack.role_id = self.attr.role_id
		pack.type = 0
		self:send_game_pack(msg.GL2GW_KICK_ROLE, pack)
		--role_mgr.remove_role( self )
		LLOGDEBUG("-----------logout_from_ga exit_scene:"..self.attr.scene_id)
		scene_mgr.leave_scene(self)
	end
	self:save(0)
	
end


function logout_from_gw( self, type )

	self:save(type == 1 and 0 or 1)
	--self.scene_id = nil
	self.gw_id = nil
	
end

function send_pack( self, msg_code, pack)
	CppNetwork.send_to_client(self.attr.gsnetid, msg_code, pack:SerializeToString(), pack:ByteSize() )
end

function send_game_pack( self, msg_code, pack)
	if self.gw_id then
		CppNetwork.send_to_gamesrv(self.gw_id,  msg_code, pack:SerializeToString(), pack:ByteSize())
	end
end

function send_to_gateway(self, msg_code, pack)
	CppNetwork.send_to_gateway(self.attr.gsnetid,msg_code, pack:SerializeToString(), pack:ByteSize())
end

-- 需要参数才加explain_list
-- 格式为{p1, p2, p3, ...}
-- p1, p2, p3 需要是string类型
function send_err_code(self, err_code, explain_list)
	code_mgr.send_code_to_client(self.attr.gsnetid, 0, err_code, explain_list)
end

function send_notice(self, notice_code, explain_list)
	code_mgr.send_code_to_client(self.attr.gsnetid, 1, err_code, explain_list)
end

function test()
	
	for k,v in pairs(role_mgr.table_role_id) do
		-- local date = os.date()
		--print("time:"..date.." _ "..os.time().." _ "..os.date("%Y/%m/%d %H:%M:%S", os.time()) )
	end
end
test()

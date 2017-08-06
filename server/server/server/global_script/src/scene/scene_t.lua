-- @version    1.0
-- @author     yangenliang
-- @date       2017-02-24 09:40:30.
-- @system     scene_t
require "res_mgr"
module( "scene_t", package.seeall )

function new(gw_id, scene_id, scene_type_id,room_type)
     local scene = { }
     setmetatable( scene, { __index = scene_t} )
     scene:init(gw_id, scene_id, scene_type_id,room_type)
     return scene
end

--初始化
----------------------------------------------------------------------
function init(self, gw_id, scene_id, scene_type_id,room_type)
	self.gw_id			= gw_id
	self.scene_id		= scene_id
	self.scene_type_id	= scene_type_id
	self.role_cnt		= 0
	self.room_type 		= room_type



	--获得房间可进人数
	if room_type==1 or room_type==2 then
		self.room_max_role_num = 1
	else
		self.room_max_role_num = 4
	end
	
end

--渔场吃的金币
function add_gold_pool(self,gold)
	self.gold_pool = self.gold_pool + gold*(1+self.tax*0.0001)
	--LLOGINFO(" scene gold_pool:"..self.gold_pool)
end
--渔场出的金币
function use_gold_pool(self,gold)
	self.gold_pool = self.gold_pool - gold
end

function add_role(self)
	self.role_cnt = self.role_cnt + 1
	--LLOGINFO(" scene add_role:"..self.role_cnt)
end

function del_role(self)
	self.role_cnt = self.role_cnt - 1
	if self.role_cnt<0 then
		LLOGINFO("err scene del_role:"..self.role_cnt)
		self.role_cnt = 0
	end
	--LLOGINFO("scene del_role:"..self.role_cnt)
end

function get_role_cnt(self)
	return self.role_cnt
end










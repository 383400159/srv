-- DO NOT EDIT, AUTO GENERATE.

require "protocol/NP_CS_Room_pb"

local NP_CS_Room_pb = NP_CS_Room_pb

module( "handler", package.seeall )
---------------------------------------------------------------------------------------------------------
----------------------------------------华丽的分割线-----------------------------------------------------
---------------------------------------------------------------------------------------------------------
			
function on_room_enter_room(role, pb_data, length)
	local msg = NP_CS_Room_pb.C2SRoomEnterRoom()
	msg:ParseFromString(pb_data)
	role.room_control:enter_room(msg.room_type_id, msg.room_type)
end

function on_room_start_refresh(role, pb_data, length)
	role.room_control:start_refresh()
end

function on_room_exit_room(role, pb_data, length)
	role.room_control:exit_room()
end

function on_room_jjj_kkk(role, pb_data, length)
	local msg = NP_CS_Room_pb.C2SRoomJjjKkk()
	msg:ParseFromString(pb_data)
	role.room_control:jjj_kkk(msg.adjhasjkd, msg.jaldajlas, msg.dasjdkahd)
end

-- DO NOT EDIT, AUTO GENERATE.

require "protocol/NP_CS_Attr_pb"

local NP_CS_Attr_pb = NP_CS_Attr_pb

module( "handler", package.seeall )
---------------------------------------------------------------------------------------------------------
----------------------------------------华丽的分割线-----------------------------------------------------
---------------------------------------------------------------------------------------------------------
			
function on_attr_jjj_kkk(role, pb_data, length)
	local msg = NP_CS_Attr_pb.C2SAttrJjjKkk()
	msg:ParseFromString(pb_data)
	role.attr_control:jjj_kkk(msg.adjhasjkd, msg.jaldajlas, msg.dasjdkahd, msg.gdfadsdas)
end

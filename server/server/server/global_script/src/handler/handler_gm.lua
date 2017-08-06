module( "handler", package.seeall )

function on_gm_cmd(role, pb_data, length)
	local accept_msg = NP_CS_Gm_pb.C2SReqGmCmd()
	accept_msg:ParseFromString(pb_data)
	local strs = util.split(accept_msg.cmd, ",")

	if nil == strs then
		return
	end

	local argc = #strs
	if argc < 1 then
		return
	end

	if strs[1] == "new_day" then
		role:on_daily_change(0,1)
		return
	end
	-----------------------------------------------------------------------------------------------

	local list = {}
	if argc > 2 then
		for i=3,#strs do
			local val = tonumber(strs[i])
			if not val then
				val = strs[i]
			end
			table.insert(list,val)
		end
	end
	local tt = util.split(strs[1],'_')
	local temp = tt[1]
	for i=2,#tt-1 do
		temp = temp..'_'..tt[i]
	end
	if not _G[strs[1]] or not _G[strs[1]][strs[2]] then
		return
	end
	_G[strs[1]][strs[2]](role[temp],unpack(list))

end

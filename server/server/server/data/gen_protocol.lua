require "./data/protocol/protocol"
package.path = package.path .. ";../scriptcommon/proto-gen-lua/?.lua"
package.path = package.path .. ";../global_script/src/protocol/?.lua"

function split(str, delimiter)
	if str==nil or str=='' or delimiter==nil then
		return nil
	end
	
    local result = {}
    for match in (str..delimiter):gmatch("(.-)"..delimiter) do
        table.insert(result, match)
    end
    return result
end

function get_first_up(str)
	return string.upper(string.sub(str, 1, 1)) .. string.lower(string.sub(str,2,#str))
end

function get_protocol_words(str)
	local tbl = split(str, "_")
	local str = get_first_up(tbl[1])
	if #tbl > 1 then
		for i=2,#tbl do
			str = str .. "_" .. get_first_up(tbl[i])
		end
	end
	return str 
end

function get_sort_key(tbl)
	local t = {}
	for k,v in pairs (tbl) do 
		table.insert(t, k)
	end
	table.sort(t)
	return t
end

function get_msg_row_str(id, direct, sys, name)
	local str = ""
	local direct = string.lower(direct)
	-- if direct == "c2s" then
	-- 	str = direct .. "_req_" .. sys .. "_" .. name
	-- elseif direct == "gl2c" then
	-- 	str = direct .. "_rep_" .. sys .. "_" .. name
	-- end
	str = direct .. "_" .. sys .. "_" .. name
	str = string.upper(str)
	local need = 35 - #str 

	for i=1,need do
		str = str .. " "
	end
	return str .. "\t=" .. string.format("%6d", id)
end

function get_func_row_str(direct, sys, name)
	local str = ""
	local direct = string.lower(direct)
	-- if direct == "c2s" then
	-- 	str = direct .. "_req_" .. sys .. "_" .. name
	-- elseif direct == "gl2c" then
	-- 	str = direct .. "_rep_" .. sys .. "_" .. name
	-- end
	str = direct .. "_" .. sys .. "_" .. name
	str = string.upper(str)
	str = "[ msg." .. str .. " ]"
	local need = 43 - #str 

	for i=1,need do
		str = str .. " "
	end
	return str .. "\t=" .. " handler.on_" .. string.lower(sys) .. "_" .. string.lower(name) .. ","
end

function get_c2srv_row_str(id, direct, sys, name)
	local str = ""
	local direct = string.lower(direct)
	-- if direct == "c2s" then
	-- 	str = direct .. "_req_" .. sys .. "_" .. name
	-- elseif direct == "gl2c" then
	-- 	str = direct .. "_rep_" .. sys .. "_" .. name
	-- end
	str = direct .. "_" .. sys .. "_" .. name
	str = string.upper(str)
	str = "DEFINE_CLIENT_MSG(" .. str .. ","
	local need = 54 - #str 

	for i=1,need do
		str = str .. " "
	end
	return str .. "\t" .. id + 5000 .. ")"
end

function get_handler_cs_name(id, direct, sys, name)
	local str = ""
	local direct = string.lower(direct)
	-- if direct == "c2s" then
	-- 	str = direct .. "_req_" .. sys .. "_" .. name
	-- elseif direct == "gl2c" then
	-- 	str = direct .. "_rep_" .. sys .. "_" .. name
	-- end
	str = direct .. "_" .. sys .. "_" .. name
	str = string.upper(str)
	local func_str = get_protocol_func(str)
	return func_str
end

function get_protocol_func(macro)
	local tbl = split(macro, "_")
	local str = tbl[1]
	for i=2,#tbl do
		str = str .. string.sub(tbl[i], 1, 1) .. string.lower(string.sub(tbl[i],2,#tbl[i]))
	end
	return str 
end

function get_msghandler_row_str(id, direct, sys, name)
	local str = ""
	local direct = string.lower(direct)
	-- if direct == "c2s" then
	-- 	str = direct .. "_req_" .. sys .. "_" .. name
	-- elseif direct == "gl2c" then
	-- 	str = direct .. "_rep_" .. sys .. "_" .. name
	-- end
	str = direct .. "_" .. sys .. "_" .. name
	str = string.upper(str)
	local func_str = get_protocol_func(str)
	str = "DEFINE_INI_PROXY_CLIENT2GL_HANDLER(" .. str .. ","
	local need = 70 - #str 

	for i=1,need do
		str = str .. " "
	end
	str = str .. "\tnet_protocol::" .. func_str  .. ","
	local need = 116 - #str
	for i=1,need do
		str = str .. " "
	end
	return str .. "Proxy2Global)"
end

function write_protocol_file()
	local state = false
	local sys_tbl = {}
	local sys_tbl_reserve = {}

	-- msg.lua
	local file_msg = io.open("../global_script/src/protocol/msg.lua", "w")
	file_msg:write([[
module( "msg", package.seeall )
dofile_ex( "../global_script/src/protocol/msg_ex" )

-------------------------------------------------------------------------------------
--客户端句柄消息 begin:15000
]])

	local sort_key = get_sort_key(protocol_def)
	local protocol_cs_tbl = {}
	for k,v in ipairs(sort_key) do
		if not sys_tbl_reserve[protocol_def[v].sys] then
			sys_tbl_reserve[protocol_def[v].sys] = 1
			protocol_cs_tbl[protocol_def[v].sys] = {}
			table.insert(sys_tbl, protocol_def[v].sys)
		end
		if protocol_def[v].id < 20000 then
			table.insert(protocol_cs_tbl[protocol_def[v].sys], protocol_def[v].id)
		end

		if protocol_def[v].id > 20000 and not state then
			file_msg:write("\n" .. [[
-------------------------------------------------------------------------------------
--返回客户端的消息(注：这里面的要和client.h里面的协议枚举一一对应)
				]]
				)
			state = true
		end
		if protocol_def[v].comment ~= "" then
			file_msg:write("\n--" .. protocol_def[v].comment .. "\n")
		end
		file_msg:write(get_msg_row_str(protocol_def[v].id, protocol_def[v].direct, protocol_def[v].sys, protocol_def[v].name) .. "\n")
	end	

	-- hander_func.lua
	local file_handler_func = io.open("../global_script/src/handler/handler_func.lua", "w")
	file_handler_func:write([[require "protocol/msg"
require "handler/handler_gm"]])
	for i,v in ipairs(sys_tbl) do
		file_handler_func:write("\nrequire \"handler/handler_" .. v .. "\"")
	end
	file_handler_func:write("\n" .. [[		
-- 客户端外部消息
game_client_func_table =
{
	[ msg.C2S_REQ_GM_CMD ]                  	= handler.on_gm_cmd,]])

	for k,v in ipairs(sort_key) do
		if v > 20000 then
			break
		end
		if protocol_def[v].comment ~= "" then
			file_handler_func:write("\n\t--" .. protocol_def[v].comment .. "\n")
		end
		file_handler_func:write("\t" .. get_func_row_str(protocol_def[v].direct, protocol_def[v].sys, protocol_def[v].name) .. "\n")
	end
	file_handler_func:write("}\n")

	-- gen handler_*.lua
	local proto = nil
	local fields = nil
	local flag = false
	local get_func = function(protocol, id)
		flag = false
		local protocol_words = get_protocol_words(protocol)
		local words_no_ = string.gsub(protocol_words, "_", "");
		local func_str = ""
		proto = require ("NP_CS_" .. protocol_words .. "_pb")
		if proto["C2S" .. string.upper(words_no_ .. get_protocol_func(protocol_def[id].name))] then
			fields = proto["C2S" .. string.upper(words_no_ .. get_protocol_func(protocol_def[id].name))].fields
			for k,v in pairs(fields) do
				flag = true
				func_str = func_str .. "msg." .. v.name .. ", "
			end
		else
			local pack = proto.C2SKNAPSACKSTALLGETLIST
			print("C2S" .. string.upper(words_no_ .. get_protocol_func(protocol_def[id].name)))

			print(pack)
			print(proto["C2S" .. string.upper(words_no_ .. get_protocol_func(protocol_def[id].name))])
			for k,v in pairs(pack.fields) do
				print(k,v.name)
			end
			print("NP_CS_" .. protocol_words .. "_pb")
			print("C2S" .. string.upper(words_no_ .. get_protocol_func(protocol_def[id].name)))
			print("protocol wrong:" .. "NP_CS_" .. protocol_words .. "_pb." 
			.. get_handler_cs_name(protocol_def[id].id, protocol_def[id].direct, protocol_words, protocol_def[id].name))
		end

		local str = "\nfunction on_" .. protocol .. "_" .. protocol_def[id].name .. "(role, pb_data, length)\n"

		if flag then
			str = str .. "	local msg = " .. "NP_CS_" .. protocol_words .. "_pb." 
			.. get_handler_cs_name(protocol_def[id].id, protocol_def[id].direct, protocol_words, protocol_def[id].name)
			.. "()\n"
			.. "	msg:ParseFromString(pb_data)\n" 			
		end
		str = str .. "	role." .. protocol .. "_control:" .. protocol_def[id].name .. "("
		if flag then
			str = str .. func_str
			str = string.sub(str, 1, #str - 2)
		end
		str = str .. ")\nend\n"
		return str
	end
	local handler_file = nil
	local func_name = nil
	for i,v in ipairs(sys_tbl) do
		handler_file = io.open("../global_script/src/handler/handler_" .. v .. ".lua", "w")
		handler_file:write("-- DO NOT EDIT, AUTO GENERATE.\n\n")
		handler_file:write("require \"protocol/" .. "NP_CS_" .. get_protocol_words(v) .. "_pb\"\n\n")
		handler_file:write("local " .. "NP_CS_" .. get_protocol_words(v) .. "_pb = " .. "NP_CS_" .. get_protocol_words(v) .. "_pb\n\n")
		handler_file:write("module( \"handler\", package.seeall )\n")
		handler_file:write(
			[[
---------------------------------------------------------------------------------------------------------
----------------------------------------华丽的分割线-----------------------------------------------------
---------------------------------------------------------------------------------------------------------
			]])
		for _i,_v in ipairs(protocol_cs_tbl[v]) do
			handler_file:write(get_func(v,_v))
		end
		handler_file:close()
	end

	----------
	-- gen c2srv_ex.h	
	local file_c2srv_ex = io.open("../servercommon/protocol/c2srv_ex.h", "w")
	file_c2srv_ex:write([[
#ifndef __C2SRV_PRO_EX_H__
#define __C2SRV_PRO_EX_H__

namespace protocol{
	namespace c2srv{
]])

	for k,v in ipairs(sort_key) do
		if v > 20000 then
			break
		end
		if protocol_def[v].comment ~= "" then
			file_c2srv_ex:write("\n\t\t//" .. protocol_def[v].comment .. "\n")
		end
		file_c2srv_ex:write("\t\t" .. get_c2srv_row_str(protocol_def[v].id, protocol_def[v].direct, protocol_def[v].sys, protocol_def[v].name) .. "\n")
	end


	file_c2srv_ex:write([[
	}
}

#endif // !__C2SRV_PRO_EX_H__
]])
	----------------------
	-- gen msghandler_ex.h
	local file_msghandler_ex = io.open("../ga_srv/msghandler/msghandler_ex.h", "w")
	file_msghandler_ex:write([[
#ifndef __MSG_HANDLER_EX_H__
#define __MSG_HANDLER_EX_H__
]])

	for i,v in ipairs(sys_tbl) do		
		file_msghandler_ex:write("\n#include \"protobuf/NP_CS_" .. get_protocol_words(v) .. ".pb.h\"")
	end


	file_msghandler_ex:write("\n\n" .. [[
static void inti_proxy_client2gl()
{
#define DEFINE_INI_PROXY_CLIENT2GL_HANDLER(code, prototype, func)\
	{\
	assert(g_client_msg_handler[protocol::c2srv::code].proto_parse_func_ == nullptr);\
	assert(g_client_msg_handler[protocol::c2srv::code].func_ == nullptr);\
	assert(func != nullptr);\
	g_client_msg_handler[protocol::c2srv::code] = protocol::MsgClientHandlerItem(&protocol::PPPHandlerFromClient<prototype>, &func);\
	}
	]])

	-- #include "protobuf/NP_CS_Task.pb.h"

	for k,v in ipairs(sort_key) do
		if v > 20000 then
			break
		end
		if protocol_def[v].comment ~= "" then
			file_msghandler_ex:write("\n\t//" .. protocol_def[v].comment .. "\n")
		end
		file_msghandler_ex:write("\t" .. get_msghandler_row_str(protocol_def[v].id, protocol_def[v].direct, protocol_def[v].sys, protocol_def[v].name) .. "\n")
	end

	file_msghandler_ex:write([[

#undef DEFINE_INI_PROXY_CLIENT2GL_HANDLER
}

#endif // !__MSG_HANDLER_EX_H__
]])

end

write_protocol_file()
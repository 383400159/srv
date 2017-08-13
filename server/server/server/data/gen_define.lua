require "./data/code/code_def"

function get_sort_key(tbl)
	local t = {}
	for k,v in pairs (tbl) do 
		table.insert(t, k)
	end
	table.sort(t)
	return t
end

function get_row_str(enum, id, explain)
	local need = 30 - #enum
	local str = enum
	for i = 1, need, 1 do
		str = str .. " "
	end
	return str .. "\t= " .. string.format("%3d",id) .. "\t--" .. explain	
end

function get_cpp_row_str(enum, id, explain)
	local need = 30 - #enum
	local str = enum
	for i = 1, need, 1 do
		str = str .. " "
	end
	return "const static int " .. str .. "\t= " .. string.format("%3d;",id) .. "\t//" .. explain	
end

function write_code_def()
	local file = io.open("../global_script/src/define/code_define.lua", "w")
	local file_cpp = io.open("../servercommon/err_code.h", "w")
	file:write("module( \"code_define\", package.seeall )\n\n--错误码\n")
	file_cpp:write([[
#ifndef _ERR_CODE_H_ 
#define _ERR_CODE_H_
	
]])
	local sort_key = get_sort_key(code_def)
	for k,v in ipairs(sort_key) do
		file:write(get_row_str(code_def[v].enum_name, code_def[v].id, code_def[v].explain) .. "\n")
		file_cpp:write(get_cpp_row_str(code_def[v].enum_name, code_def[v].id, code_def[v].explain) .. "\n")
	end	
	file:write("\n--公告\n")
	sort_key = get_sort_key(notice_def)
	for k,v in ipairs(sort_key) do
		file:write(get_row_str(notice_def[v].enum_name, notice_def[v].id, notice_def[v].explain) .. "\n")	
		file_cpp:write(get_cpp_row_str(notice_def[v].enum_name, notice_def[v].id, notice_def[v].explain) .. "\n")
	end
	file_cpp:write([[		
#endif // _ERR_CODE_H_ 
		]])
    file:close()
    file_cpp:close()
end

write_code_def()

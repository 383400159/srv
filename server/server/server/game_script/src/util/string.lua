module ( "util", package.seeall )

function split(str, delim, maxNb)
	-- Eliminate bad cases...
	if not str then
		print(debug.traceback())
		return 
	end
	if string.find(str, delim) == nil then
		return { str }
	end
	if maxNb == nil or maxNb < 1 then
		maxNb = 0    -- No limit
	end
	local result = {}
	local pat = "(.-)" .. delim .. "()"
	local nb = 0
	local lastPos
	for part, pos in string.gfind(str, pat) do
		nb = nb + 1
		result[nb] = part
		lastPos = pos
		if nb == maxNb then break end
	end
	-- Handle the last field
	if nb ~= maxNb then
		result[nb + 1] = string.sub(str, lastPos)
		if string.len(result[nb + 1]) == 0 then
			result[nb + 1] = nil
		end
	end
	return result
end

-- example:
-- local s = 8051#10|8052#9 -- item_id#count|item_id#count
-- split_item_count(s, "|", "#", check_item_by_id_func)
-- @return t = {
-- 	[1] = {
-- 			[1] = 8051, 
--			[2] = 10,	
--			},
-- 	[2] = {
-- 			[1] = 8052, 
--			[2] = 9,	
--			},
-- }

function split_item_count(str, fc, sc, check_item_by_id_func)
	local item = nil
	local items = nil 
	local ret_tbl = {}

	items = split(str, fc)
	for i = 1, #items do
		item = split(items[i], sc)
		if 2 ~= #item then
			error ("split string data error, string:" .. str)
		end
		if nil ~= check_item_by_id_func and false == check_item_by_id_func(tonumber(item[1])) then
			error ("load data error, string:" .. str)
		end
		ret_tbl[i] = {}
		ret_tbl[i][1] = tonumber(item[1])
		ret_tbl[i][2] = tonumber(item[2])
	end
	return ret_tbl
end


function split_str_str(str, fc, sc)
	local item = nil
	local items = nil 
	local ret_tbl = {}

	items = split(str, fc)
	for i = 1, #items do
		item = split(items[i], sc)
		if 2 ~= #item then
			error ("split string data error, string:" .. str)
		end
		ret_tbl[i] = {}
		ret_tbl[i][1] = item[1]
		ret_tbl[i][2] = item[2]
	end
	return ret_tbl
end

-- @ split_hour_min("12:30")
function split_hour_min(str)
	local time_tbl = split_item_count(str, "|", ":")
	time_tbl.hour  = time_tbl[1][1]
	time_tbl.min   = time_tbl[1][2]
	time_tbl[1] = nil
	return time_tbl
end

-- @ split drop list
-- eg:1004,2,3,50;1004,4,5,20|1004,21,21,100|1004,1,1,100
-- out table
--[[
t = {
	[1] = { -- grop
		[1] = { -- index
			[1] = 1004, -- id
			[2] = 2, 	-- intensify level
			[3] = 3,	-- cnt
			[4] = 50,	-- prob
		},
		[2] = {
			[1] = 1004,
			[2] = 4,
			[3] = 5,
			[4] = 20,	
		},	
	},
	[2] = {
		[1] = {
			[1] = 1004,
			[2] = 21,
			[3] = 21,
			[4] = 100,	
		},	
	},
	[3] = {
		[1] = {
			[1] = 1004,
			[2] = 1,
			[3] = 1,
			[4] = 100,	
		},	
	},
}
--]]
function split_drop_list(str)
	local arr = split(str, "|")
	for k,v in pairs(arr) do
		arr[k] = split(v, ";")
		for _k,_v in pairs(arr[k]) do
			arr[k][_k] = split(_v, ",")
		end
	end
	return arr
end
-- @ get drop list
-- out table
--[[
t = {
	[1] = {
		[1] = 1004, -- id
		[2] = 2,	-- intensify level
		[3] = 3,	-- cnt
	},
	[2] = {
		[1] = 1004, 
		[2] = 2,
		[3] = 3,
	
	},
	[3] = {
		[1] = 1004, 
		[2] = 2,
		[3] = 3,
	
	},
}
--]]
function get_monster_drop_list(drop_tbl)
	local drop = {}
	local rate = 0
	local curr_rate = 0
	for id,group in pairs(drop_tbl) do
		local arr = {}
		rate = math.random(0, 10000)
		curr_rate = 0
		for k,v in pairs(group) do
			curr_rate = curr_rate + v[4]
			if rate <= curr_rate then
				arr[1] = v[1] 
				arr[2] = v[2] 
				arr[3] = v[3]
				table.insert(drop, arr) 
				break
			end
		end
	end
	return drop
end

-- test
-- local a = split_drop_list("1004,2,3,5000;1004,4,5,2000|1004,21,21,1000|1004,1,1,1000")
-- local t = get_monster_drop_list(a)
-- for k,v in pairs(t) do
-- 	print(k,v[1], v[2], v[3])
-- end


function get_row(tbl)
	local drop = {}
	local rate = 0
	local curr_rate = 0
	for id,group in ipairs(tbl) do
		local arr = {}
		rate = math.random(0,10000)
		curr_rate = 0
		for i,v in ipairs(group) do
			if 1 == v[6] then
				curr_rate = curr_rate + v[5]
				if rate <= curr_rate then
					arr[1] = v[1]
					arr[2] = v[2]
					arr[3] = v[3]
					arr[4] = v[4]
					table.insert(drop, arr) 
					break
				end
			elseif 2 == v[6] then
				if v[4] == math.random(1,v[4]) then
					arr[1] = v[1]
					arr[2] = v[2]
					arr[3] = v[3]
					arr[4] = v[4]
					table.insert(drop, arr) 
					break
				end
			end
		end
	end
	return drop
end

function recalc_item_count(drop_tbl)
	for k,v in pairs(drop_tbl) do
		for _k,_v in pairs(v) do
			_v.money = get_row(_v[0] or {})
			_v.material = get_row(_v[1] or {})
		end
	end
end

function get_drop_by_sex_prof(tbl, prof, sex, is_recalc)
	if is_recalc then
		recalc_item_count(tbl)
	end
	local praise = tbl
	local output_data = {money = {}, material = {},}

	local insert_func = function(tbl)
		for k,v in pairs(tbl.money) do
			table.insert(output_data.money, v)
		end
		for k,v in pairs(tbl.material) do
			table.insert(output_data.material, v)
		end
	end

	if praise[0] then
		if praise[0][0] then
			insert_func(praise[0][0])
		end
		if praise[0][sex] then
			insert_func(praise[0][sex])
		end
	end
	if praise[prof] then
		if praise[prof][0] then
			insert_func(praise[prof][0])
		end
		if praise[prof][sex] then
			insert_func(praise[prof][sex])
		end
	end
	return output_data
end

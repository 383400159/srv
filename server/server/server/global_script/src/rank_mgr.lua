module( "rank_mgr", package.seeall )
require "role_mgr"
local role_mgr = role_mgr
--记录一些排行数据
rank_mgr_table = rank_mgr_table or {}
--记录哪些排行需要重新加载
rank_res_table = rank_res_table or {}
--记录已有排行 避免重复加载
rank_ret_table = rank_ret_table or {}
--记录下一次刷新排行榜的时间
next_res_time = 0
is_init = true
is_save = false

--type: 1捕鱼总分 2连击 3金币 4土豪
function add_role_rank(user_name,type,rank_tb,replace)
	--LLOGINFO("add_role_rank:"..user_name.." "..type)
	if replace==nil then
		--默认替换
		replace = 1
	end
	if rank_mgr_table[type]==nil then
		rank_mgr_table[type] = {}
	end

	local tb = rank_mgr_table[type]

	if replace==1 then
		rank_tb['user_name'] = user_name
		tb[user_name] = rank_tb
	elseif replace==2 then
		--取最大值
		rank_tb['user_name'] = user_name
		if tb[user_name]==nil then
			tb[user_name] = rank_tb
		end
		if tb[user_name].score<rank_tb.score then
			tb[user_name].score = rank_tb.score
		end
	elseif replace==3 then
		--相加
		rank_tb['user_name'] = user_name
		if tb[user_name]==nil then
			tb[user_name] = rank_tb
		end
		tb[user_name].score = tb[user_name].score + rank_tb.score
		
	else
		--其他处理规则
	end

	--需要重新加载
	rank_res_table[type] = 1
	is_save = true
end


--排序示例
-- local function testSort(a,b)
--         return a.level> b.level
-- end

function get_rank_by_type(type,func)
	if func==nil then
		func = function (a,b) return a.score>b.score end
	end
	--LLOGINFO("get_rank_by_type:"..type)
	local ret_tb = {}
	local tb = rank_mgr_table[type]
	if tb==nil then tb = {} end

	if rank_res_table[type]==1 and os.time()>next_res_time then
		next_res_time = next_res_time + 300
		--重新刷新一份新的列表
		local new_tb = {}
		for k,v in pairs(tb) do
			new_tb[#new_tb+1] = v
		end

		table.sort(new_tb,func)


	    --排序完了以后需要剔除最大值以上的东西 
	    if  #new_tb>50 then
	    	for i=51,#new_tb do
	    		if new_tb[i]~=nil then
	    			tb[new_tb[i].user_name]=nil
	    			new_tb[i]=nil	
	    		end
	    	end
	    end
		rank_ret_table[type] = new_tb	
		
		--向玩家推送主动更新
		for _, v in pairs(role_mgr.table_role_id) do
			v.rank_control:send()
		end
	end

	ret_tb = rank_ret_table[type]

	-- for k,v in pairs(ret_tb) do
	-- 	LLOGINFO(k.." "..v.role_id.." "..v.score)
	-- end


	return ret_tb
end

--获得玩家所在排行榜的位置
function get_role_rank_num(type,user_name)
	local tb = rank_ret_table[type]
	for k,v in pairs(tb) do
		if v.user_name==user_name then
			return k
		end
	end
	return 99999
end


function update( interval, day_id)
	
end

function load_db_rankMgrTable(tbl_str)
	if tbl_str == nil then return end
	--LLOGINFO("load_db_rankMgrTable:"..tbl_str)
	local t = util.unserialize(tbl_str)
	if t~=nil then
		rank_mgr_table = t
	end
end

function load_db_rankResTable(tbl_str)
	if tbl_str == nil then return end
	--LLOGINFO("load_db_rankResTable:"..tbl_str)
	local t = util.unserialize(tbl_str)
	if t~=nil then
		rank_res_table = t
	end
end

function load_db_rankRetTable(tbl_str)
	if tbl_str == nil then return end
	--LLOGINFO("load_db_rankRetTable:"..tbl_str)
	local t = util.unserialize(tbl_str)
	if t~=nil then
		rank_ret_table = t
	end
end




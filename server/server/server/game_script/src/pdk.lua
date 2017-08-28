module ( "pdk", package.seeall )

local Card = {"3","4","5","6","7","8","9","10","J","Q","K","A","2"}
local random = math.random
local sort = table.sort


--测试入口 play传入类型决定要测试的类型 CppSrv.DebugLog 换成print 或者封装的c++ log接口即可
function run()
	--playdata 及 tipsdata 组成 例如:{2,3,3} 代表{双牌,5,5} 第一个代表类型
	local playdata = play(7)
	local playstr = tableToStr(playdata)
	CppSrv.DebugLog("pdk play:"..playstr)


	local mylist,mylistcount = createCardList()
	local mystr = tableToStr1(mylist)
	CppSrv.DebugLog("pdk myCardList:"..mystr)

	--选完牌 开始做提示
	local tipsdata = tips(playdata,mylistcount)
	local tipsstr = tableToStr(tipsdata)
	CppSrv.DebugLog("pdk tips:"..tipsstr)
end

--显示手牌
function tableToStr1(t)
	local str = ""
	
	for i=1,#t do
		--str = str..","..t[i].."("..Card[t[i]]..")"
		str = str..","..Card[t[i]]
	end

	return str
end

--显示
function tableToStr(t)
	--local str = t[1]
	local str = ""
	if #t>1 then
		for i=2,#t do
			--str = str..","..t[i].."("..Card[t[i]]..")"
			str = str..","..Card[t[i]]
		end
	end
	
	if #t==1 then
		str = str..",pass"
	end
	
	return str
end

function comps(a,b)
	return a < b
end

--生成一副16张长度的牌
function createCardList()
	local list = {}
	local list_count = {}

	local i = 0
	while i<16 do
		local r = random(1,#Card)
		local run = true
		if (r==12 and list_count[r]==3) or (r==13 and list_count[r]==1)  then
			run = false
		end
		if (list_count[r]==nil or  list_count[r]<4) and run then
			list[#list+1] = r
			list_count[r] = list_count[r]==nil and 1 or list_count[r] + 1 
			i = i + 1
		end
	end

	--排序
	sort(list,comps);
	return list,list_count
end

--出牌方 忽视这个play的bug 我们只是拿来测试
function play(type)
	local r = type
	local retdata = {r}
	--出单张
	if r==1 then
		retdata[#retdata+1]=random(1,#Card)
	end
	--出对子
	if r==2 then
		local p = random(1,#Card)
		for i=1,2 do
			retdata[#retdata+1] = p
		end
	end
	--出连对
	if r==3 then
		local p = random(1,#Card)
		for i=1,2 do
			retdata[#retdata+1] = p
		end
		--这个有肯定bug 测试没处理
		for i=1,2 do
			retdata[#retdata+1] = p + 1
		end
	end
	--出三带一
	if r==4 then
		local p = random(1,#Card)
		for i=1,3 do
			retdata[#retdata+1] = p
		end

		--这里没处理相同牌 可能变炸弹~~~
		local p = random(1,#Card)
		retdata[#retdata+1] = p
		
	end
	--出四带二
	if r==5 then
		local p = random(1,#Card)
		for i=1,4 do
			retdata[#retdata+1] = p
		end

		--同上 可能会出6张相同牌的bug
		local p = random(1,#Card)
		for i=1,2 do
			retdata[#retdata+1] = p
		end
	end
	--出炸弹
	if r==6 then
		local p = random(1,#Card)
		for i=1,4 do
			retdata[#retdata+1] = p
		end
	end
	--出顺子
	if r==7 then
		--顺子太容易突破上限了 我们这写死几组做测试
		local test = {{2,3,4,5,6},{5,6,7,8,9}}
		local tr = random(1,#test)
		for k,v in pairs(test[tr]) do
			retdata[#retdata+1] = v
		end
	end

	return retdata
end

--提示主循环
function tips(t,t_count)
	local r = t[1]
	local retdata = {r}
	--这个all_tips 装所有的可能 实际项目中 生成一次供玩家选择即可
	local all_tips = {}
	--单张提示
	if r==1 then
		for k,v in pairs(t_count) do
			if k>t[2] and t_count[k]~=nil then
				all_tips[#all_tips+1] = {k}
			end
		end

	end

	--对牌提示
	if r==2 then
		for k,v in pairs(t_count) do
			if k>t[2] and (t_count[k]~=nil and t_count[k]>=2) then
				all_tips[#all_tips+1] = {k,k}
			end
			--2做万能牌
			if k>t[2] and t_count[13]~=nil and (t_count[k]~=nil and t_count[k]==1) then
				all_tips[#all_tips+1] = {k,13}
			end
		end
	end

	--连对提示
	if r==3 then
		for k,v in pairs(t_count) do
			if k>t[2] and (t_count[k]~=nil and t_count[k]>=2) and (t_count[k+1]~=nil and t_count[k+1]>=2) then
				--需要判断最大值
				all_tips[#all_tips+1] = {k,k,k+1,k+1}
			end

			--2做万能牌
			if k>t[2] and t_count[13]~=nil and (t_count[k]~=nil and t_count[k]==1) and (t_count[k+1]~=nil and t_count[k+1]>=2) then
				all_tips[#all_tips+1] = {k,13,k+1,k+1}
			end
			if k>t[2] and t_count[13]~=nil and (t_count[k]~=nil and t_count[k]>=2) and (t_count[k+1]~=nil and t_count[k+1]==1) then
				all_tips[#all_tips+1] = {k,k,k+1,13}
			end
		end

	end

	--三带一提示
	if r==4 then
		for k,v in pairs(t_count) do
			if k>t[2] and (t_count[k]~=nil and t_count[k]>=3) then
				all_tips[#all_tips+1] = {k,k,k,nothis(t_count,t[2])}
			end
			--2做万能牌
			if k>t[2] and t_count[13]~=nil and (t_count[k]~=nil and t_count[k]==2) then
				all_tips[#all_tips+1] = {k,k,13,nothis(t_count,t[2],true)}
			end
		end
	
	end

	--出四带二
	if r==5 then
		for k,v in pairs(t_count) do
			if k>t[2] and (t_count[k]~=nil and t_count[k]>=4)  then
				all_tips[#all_tips+1] = {k,k,k,k,nothis(t_count,t[2]),nothis(t_count,t[2])}
			end
			--2做万能牌
			if k>t[2] and (t_count[k]~=nil and t_count[k]==3)  then
				all_tips[#all_tips+1] = {k,k,k,13,nothis(t_count,t[2],true),nothis(t_count,t[2],true)}
			end
		end
		
	end

	--出炸弹 
	if r==6 then
		for k,v in pairs(t_count) do
			if k>t[2] and (t_count[k]~=nil and t_count[k]>=4)  then
				all_tips[#all_tips+1] = {k,k,k,k}
			end

			--2做万能牌
			if k>t[2] and t_count[13]~=nil and (t_count[k]~=nil and t_count[k]==3)  then
				all_tips[#all_tips+1] = {k,k,k,13}
			end
		end
	end

	--出顺子
	if r==7 then
		for k,v in pairs(t_count) do
			if k>t[2] and Card[k+4]~=nil and t_count[k]~=nil and t_count[k+1]~=nil and t_count[k+2]~=nil and t_count[k+3]~=nil and t_count[k+4]~=nil then
				all_tips[#all_tips+1] = {k,k+1,k+2,k+3,k+4}
			end
			--2做万能牌
			if k>t[2] and t_count[13]~=nil and Card[k+4]~=nil and Card[k+4]~="2" and t_count[k]==nil and t_count[k+1]~=nil and t_count[k+2]~=nil and t_count[k+3]~=nil and t_count[k+4]~=nil then
				all_tips[#all_tips+1] = {13,k+1,k+2,k+3,k+4}
			end
			if k>t[2] and t_count[13]~=nil and Card[k+4]~=nil and Card[k+4]~="2" and t_count[k]~=nil and t_count[k+1]==nil and t_count[k+2]~=nil and t_count[k+3]~=nil and t_count[k+4]~=nil then
				all_tips[#all_tips+1] = {k,13,k+2,k+3,k+4}
			end
			if k>t[2] and t_count[13]~=nil and Card[k+4]~=nil and Card[k+4]~="2" and t_count[k]~=nil and t_count[k+1]~=nil and t_count[k+2]==nil and t_count[k+3]~=nil and t_count[k+4]~=nil then
				all_tips[#all_tips+1] = {k,k+1,13,k+3,k+4}
			end
			if k>t[2] and t_count[13]~=nil and Card[k+4]~=nil and Card[k+4]~="2" and t_count[k]~=nil and t_count[k+1]~=nil and t_count[k+2]~=nil and t_count[k+3]==nil and t_count[k+4]~=nil then
				all_tips[#all_tips+1] = {k,k+1,k+2,13,k+4}
			end
			if k>t[2] and t_count[13]~=nil and Card[k+4]~=nil and t_count[k]~=nil and t_count[k+1]~=nil and t_count[k+2]~=nil and t_count[k+3]==nil and t_count[k+4]==nil then
				all_tips[#all_tips+1] = {k,k+1,k+2,k+3,13}
			end
		end
	end

	--通用炸弹
	if r~=6 then
		--够4个一样就炸
		for k,v in pairs(t_count) do
			if t_count[k]==4 then
				all_tips[#all_tips+1] = {k,k,k,k}
			end

			--2做万能牌
			if t_count[k]==3 and t_count[13]~=nil then
				all_tips[#all_tips+1] = {k,k,k,13}
			end
		end
	end

	--这里面装有所有的可解集合
	if #all_tips>0 then
		local ar = random(1,#all_tips)
		for k,v in pairs(all_tips[ar]) do
			retdata[#retdata+1] = v
		end
	end
	return retdata
end

--随机赛选出一张和自己无关的牌 notwo不能再推送2的万能牌
function nothis(t,num,notwo)
	if notwo==nil then notwo = false end

	local data = {}
	for k,v in pairs(t) do
		if k~=num and (not notwo or k~=13) then
			data[#data+1] = k
		end
	end
	local r = random(1,#data)
	return data[r]
end
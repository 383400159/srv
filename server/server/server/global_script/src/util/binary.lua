module ( "util", package.seeall )

--十进制数字转换为二进制位列表
function binary2list( num )
	if not num then return {} end
	num = tonumber(num)
	local ret = {}
	local i = 1

	while 0 ~= num do
		ret[i] = num % 2
		num = math.floor(num/2)

		if 0 == num then break end
		i = i + 1
	end
	return ret
end

function list2binary(list)
	local ret = 0
	local count = #list
	for i = count, 1, -1 do
		ret = ret + tonumber(list[i])

		if 1 == i then break end
		ret = ret * 2
	end
	return ret
end

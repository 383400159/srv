local mem_monitor_tlb = mem_monitor_tlb or {}
local rollback_tbl = rollback_tbl or {}

function itr_table(tlb)
	local name = nil
	rawset(tlb, "_@Itr_", 1)
	for key, value in pairs(tlb) do
		if type(value) == "table" then
			if rawget(value,"_@Itr_") == nil then
				name = rawget(value,"_@name_")
				if name ~= nil then
					if mem_monitor_tlb[name] == nil then
						mem_monitor_tlb[name] = 1
					else
						mem_monitor_tlb[name] = mem_monitor_tlb[name] + 1
					end
				end
				itr_table(value)
			end
		end
	end

	table.insert(rollback_tbl, tlb)
end

function mem_monitor()
	itr_table(_G)

	for key, value in pairs(mem_monitor_tlb) do
		LLOGINFO("[" .. key .. ":" .. value .. "]")
	end 

	for _, value in pairs(rollback_tbl) do
		rawset(value, "_@Itr_", nil)
	end
	mem_monitor_tlb = {}
	rollback_tbl = {}
end

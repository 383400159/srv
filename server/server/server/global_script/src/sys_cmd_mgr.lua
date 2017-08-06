module( "sys_cmd_mgr", package.seeall )
next_res_time = 0

function cmd_call(cmd)
    local strs = util.split(cmd, ",")
    if nil == strs then
        return
    end

    local argc = #strs
    if argc < 1 then
        return
    end

    if strs[1] == "reload" then
        local params = {}
        for i = 3, #strs do
            params[i - 2] = strs[i]
        end
        update_mgr.reload(tonumber(strs[2]), params)
    end

    if strs[1] == "update_msg_status" then --开关消息调用频率打印(正式版本慎用)
        local status = tonumber(strs[2]) or 0
        CppSrv.set_msg_info_status(status)
    end
end

function cmd_call_by_dos(cmd)
    local strs = util.split(cmd, ",")

    if nil == strs then
        return
    end

    local argc = #strs

    if argc < 1 then
        return
    end

    if strs[1] == "reload" then
        CppLua.reload_all_script()
        print("-----------------------------cmd_call_by_dos----------------------")
        return
    end
    if strs[1] == "update_msg_status" then --开关消息调用频率打印(正式版本慎用)
        local status = tonumber(strs[2]) or 0
        CppSrv.set_msg_info_status(status)
    end
end



--
--  $Id: package_ex.lua 328 2011-03-30 09:30:31Z swordhell $
--
-- 介绍：
-- 		设置脚本环境 		
--
--------------------------------------------------------------------------------

--服务器启动时会自动调用的lua脚本，做初始化工作
g_debug = true
--if not g_debug then
--    package.path = "./?.lc"
--    --[[
--    LLOG("*********************LUA RELEASE START********************")
--    LLOG("[START] SERVER load script *.lc.")
--    ]]
--else 
--    package.path = "./?.lua"
--    --[[
--    LLOG("++++++++++++++++++++++LUA DEBUG START++++++++++++++++++++++")
--    LLOG("[START] SERVER DEBUG VER only load script *.lua.")
--    ]]
--end

--
-- $Id: main.lua 46136 2012-08-01 09:06:02Z abel $
--
-- 模块名：
--     主启动模块 
-- 介绍：
--     加载脚本代码 
-- 外部接口：
--     require_ex               加载脚本模块
--     dofile_ex                加载脚本到别的模块(将匿名的thunk加入到本模块)
--
--------------------------------------------------------------------------------

--设置lua环境
function add_path_to_env( path )
    print("old path:"..package.path.."\n")
    package.path = package.path .. path
    print("path:"..package.path.."\n")
end

function start_srv_begin()
    if g_is_starting_srv == nil then
        g_is_starting_srv = true
    end
end

function start_srv_end()
    if nil ~= g_is_starting_srv then
        g_is_starting_srv = false
    end
end

--强制重新载入module
function require_ex( mname )
    --[[if package.loaded[mname] then
        if not g_is_starting_srv then
            CppLog.log( CppLog.LL_INFO, string.format("require_ex module[%s] reload", mname))
        end
    end--]]
    package.loaded[mname] = nil
    require( mname )
end

--加载文件到其他模块
function dofile_ex( filename )
    local func, err = loadfile( filename .. ".lua" )
    if func == nil then
        error("[SCRIPT](lua) dofile_ex: fail to load " .. filename .. "\n" .. err .. "\n" .. debug.traceback())
        return
    end
    setfenv( func, getfenv( 2 ) )
    func()
end

start_srv_begin()

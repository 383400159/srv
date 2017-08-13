
-------------------------------------------
--  c++模块里面导出的函数，在这里做lua的包装
--
-------------------------------------------

function LLOGINFO( str )
    CppLog.log( CppLog.LL_INFO, str )
end

function LLOGERROR( str )
    CppLog.log( CppLog.LL_ERROR, str .. "\n" .. debug.traceback())
end

function LLOGERROR_NOLOG( str )
    CppLog.log( CppLog.LL_ERROR, str .. "\n")
end

function LLOGDEBUG( str )
    CppLog.log( CppLog.LL_DEBUG, str .. "\n" .. debug.traceback())
end

function new_tbl( name )
	return {["_@name_"] = name}
end


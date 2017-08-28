module ( "xiecheng", package.seeall )

function run()
	-- local coco = coroutine.create(
	-- 	function (a,b)
	-- 	    print("resume args:"..a..","..b)
	-- 	    yreturn = coroutine.yield()
	-- 	    print ("yreturn :"..yreturn)
	-- 	end
	-- )
	-- coroutine.resume(coco,0,1)
	-- coroutine.resume(coco,21)

	-- coco2 = coroutine.wrap(
	-- 	function (a,b)
	-- 	    print("resume args:"..a..","..b)
	-- 	    yreturn = coroutine.yield()
	-- 	    print ("yreturn :"..yreturn)
	-- 	end
	-- )
	-- print(type(coco2))
	-- coco2(0,1)
	-- coco2(21)

	function foo(a)
	    print("foo", a)
	    return coroutine.yield(2 * a)
	end

	co = coroutine.create(function ( a, b )
	    print("co-body", a, b)
	    local r = foo(a + 1)
	    print("co-body", r)
	    local r, s = coroutine.yield(a + b, a - b)
	    print("co-body", r, s)
	    return b, "end"
	end)

	print("main", coroutine.resume(co, 1, 10))
	print("main", coroutine.resume(co, "a"))
	print("main", coroutine.resume(co, "x", "y"))
	print("main", coroutine.resume(co, "x", "y"))
end
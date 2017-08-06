print("res_mgr.lua")
module( "res_mgr", package.seeall )

dofile_ex "../data/data/level" 
dofile_ex "../data/data/base_attribute" 
dofile_ex "../data/data/lotterybox" 
dofile_ex "../data/data/store"
dofile_ex "../data/data/misc" 
dofile_ex "../data/data/role_lv" 
dofile_ex "../data/data/gun" 
dofile_ex "../data/data/gun_unlock" 
dofile_ex "../data/data/fish" 
dofile_ex "../data/data/island" 
dofile_ex "../data/data/skill" 
dofile_ex "../data/data/status" 
dofile_ex "../data/data/mail" 
dofile_ex "../data/data/item" 
dofile_ex "../data/data/sign" 
dofile_ex "../data/data/lottery_rotary" 
dofile_ex "../data/data/task" 
dofile_ex "../data/data/notice" 


is_release_version_ = false


function check_res()

	return true
end


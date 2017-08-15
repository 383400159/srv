PROJECT("libtolua++")
#相关头文件的目录
INCLUDE_DIRECTORIES(../3rd/lua5.1/include)
#相关库文件的目录
LINK_DIRECTORIES(../3rd/lua5.1/v64_linux/lib)

FILE(GLOB_RECURSE TOLUA_FILES "../3rd/tolua++/src/*.c")
FILE(GLOB_RECURSE PROTO_GEN_LUA "../3rd/protoc_gen_lua/*.c")


ADD_LIBRARY(tolua++ ${TOLUA_FILES} ${PROTO_GEN_LUA})
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")  
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")  
SET(LIBRARY_OUTPUT_PATH ../bin/lib )


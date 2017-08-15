# 目标文件名
TARGET		= libgame.so

# 头文件包含目录
INC_DIR		+= -I./
INC_DIR		+= -I../res
INC_DIR		+= -I../3rd/libfirefly
INC_DIR		+= -I../3rd/boost/include
INC_DIR		+= -I../3rd/msgpack/include
INC_DIR		+= -I../3rd/protobuf/include
INC_DIR		+= -I../3rd/lua5.1/include
INC_DIR		+= -I../3rd/tolua++/src

# 库文件引用目录
LIB_DIR		+= -L../bin/lib
LIB_DIR		+= -L../3rd/boost/lib
LIB_DIR		+= -L../3rd/protobuf/lib
LIB_DIR		+= -L../3rd/lua5.1/lib

# 源文件目录
SRC_DIR		+= .
SRC_DIR		+= ./fishserver
SRC_DIR		+= ./fishserver/script
SRC_DIR		+= ./fishserver/scene
SRC_DIR		+= ./fishserver/object
SRC_DIR		+= ./fishserver/func
SRC_DIR		+= ./fishserver/fishgroup
SRC_DIR		+= ./res
SRC_DIR		+= ./res/bullet
SRC_DIR		+= ./res/camera
SRC_DIR		+= ./res/camera_fish
SRC_DIR		+= ./res/fish
SRC_DIR		+= ./res/fishgroup
SRC_DIR		+= ./res/gun
SRC_DIR		+= ./res/gun_unlock
SRC_DIR		+= ./res/level
SRC_DIR		+= ./res/lotterybox
SRC_DIR		+= ./res/path
SRC_DIR		+= ./lua
SRC_DIR		+= ../res/3dfish/protocol

SO_DIR	    += ./so

# 库文件
LIB_LIST	+= -l res
LIB_LIST	+= -l lua 
LIB_LIST	+= -l tolua++
LIB_LIST	+= -l protobuf
LIB_LIST	+= -l boost_system
LIB_LIST	+= -l boost_filesystem
LIB_LIST	+= -l pthread
LIB_LIST	+= -l dl


CC			= g++
CCFLAGS 	= -std=c++11 -Wall -Wno-format-security -ggdb -D GLIB_CXX_USE_CXX11_ABI=0 -fPIC -shared 
LDFLAGS     =  -fPIC -shared -Wl,--no-undefined -Wl,-rpath=$(SO_DIR)

# 二进制文件输出目录
OUT_DIR		= ../bin/plugin
OBJ_DIR		= ../obj/$(TARGET)

BIN_TARGET 	= $(OUT_DIR)/$(TARGET)

SRC_LIST 	= $(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.cc))
SRC_LIST 	+= $(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.cpp))
OBJ_LIST	= $(addsuffix .o,$(addprefix $(OBJ_DIR)/,$(basename $(notdir $(SRC_LIST)))))
DEP_LIST	= $(addsuffix .d,$(OBJ_LIST))

.PHONY : all
all: start prepare build done

.PHONY : clean
clean:
	@rm -rf $(OBJ_DIR)
	@rm -rf $(OUT_DIR)/$(TARGET)

.PHONY : start
start:
	@echo ==== Start. $(shell date) ====

.PHONY : prepare
prepare:
	@mkdir -p $(OUT_DIR)
	@mkdir -p $(OBJ_DIR)

.PHONY : build
build : $(BIN_TARGET)

.PHONY : done
done:
	@echo ==== Done. $(shell date) ====

$(BIN_TARGET) : $(OBJ_LIST)
	@echo "linking..."
	# @echo $(CC)
	# @echo $(LDFLAGS)
	# @echo $(OBJ_LIST)
	# @echo $(LIB_DIR)
	# @echo $(LIB_LIST)
	@$(CC) $(LDFLAGS) -o $@ $(OBJ_LIST) $(LIB_DIR) $(LIB_LIST)

define make-cmd-cc
$2 : $1
	$$(info compile $$<)
	@$(CC) $$(CCFLAGS) $(INC_DIR) -MMD -MT $$@ -MF $$@.d -c -o $$@ $$<
endef

$(foreach afile, $(SRC_LIST),\
	$(eval $(call make-cmd-cc,$(afile),\
		$(addsuffix .o, $(addprefix $(OBJ_DIR)/, $(basename $(notdir $(afile))))))))

-include $(DEP_LIST)

-- Generated By protoc-gen-lua Do not Edit
local protobuf = require "protobuf"
module('NP_CS_Room_pb')


C2SROOMENTERROOM = protobuf.Descriptor();
local C2SROOMENTERROOM_ROOM_TYPE_ID_FIELD = protobuf.FieldDescriptor();
local C2SROOMENTERROOM_ROOM_TYPE_FIELD = protobuf.FieldDescriptor();
C2SROOMSTARTREFRESH = protobuf.Descriptor();
C2SROOMEXITROOM = protobuf.Descriptor();
C2SROOMJJJKKK = protobuf.Descriptor();
local C2SROOMJJJKKK_ADJHASJKD_FIELD = protobuf.FieldDescriptor();
local C2SROOMJJJKKK_JALDAJLAS_FIELD = protobuf.FieldDescriptor();
local C2SROOMJJJKKK_DASJDKAHD_FIELD = protobuf.FieldDescriptor();

C2SROOMENTERROOM_ROOM_TYPE_ID_FIELD.name = "room_type_id"
C2SROOMENTERROOM_ROOM_TYPE_ID_FIELD.full_name = ".net_protocol.C2SRoomEnterRoom.room_type_id"
C2SROOMENTERROOM_ROOM_TYPE_ID_FIELD.number = 1
C2SROOMENTERROOM_ROOM_TYPE_ID_FIELD.index = 0
C2SROOMENTERROOM_ROOM_TYPE_ID_FIELD.label = 2
C2SROOMENTERROOM_ROOM_TYPE_ID_FIELD.has_default_value = false
C2SROOMENTERROOM_ROOM_TYPE_ID_FIELD.default_value = 0
C2SROOMENTERROOM_ROOM_TYPE_ID_FIELD.type = 13
C2SROOMENTERROOM_ROOM_TYPE_ID_FIELD.cpp_type = 3

C2SROOMENTERROOM_ROOM_TYPE_FIELD.name = "room_type"
C2SROOMENTERROOM_ROOM_TYPE_FIELD.full_name = ".net_protocol.C2SRoomEnterRoom.room_type"
C2SROOMENTERROOM_ROOM_TYPE_FIELD.number = 2
C2SROOMENTERROOM_ROOM_TYPE_FIELD.index = 1
C2SROOMENTERROOM_ROOM_TYPE_FIELD.label = 2
C2SROOMENTERROOM_ROOM_TYPE_FIELD.has_default_value = false
C2SROOMENTERROOM_ROOM_TYPE_FIELD.default_value = 0
C2SROOMENTERROOM_ROOM_TYPE_FIELD.type = 13
C2SROOMENTERROOM_ROOM_TYPE_FIELD.cpp_type = 3

C2SROOMENTERROOM.name = "C2SRoomEnterRoom"
C2SROOMENTERROOM.full_name = ".net_protocol.C2SRoomEnterRoom"
C2SROOMENTERROOM.nested_types = {}
C2SROOMENTERROOM.enum_types = {}
C2SROOMENTERROOM.fields = {C2SROOMENTERROOM_ROOM_TYPE_ID_FIELD, C2SROOMENTERROOM_ROOM_TYPE_FIELD}
C2SROOMENTERROOM.is_extendable = false
C2SROOMENTERROOM.extensions = {}
C2SROOMSTARTREFRESH.name = "C2SRoomStartRefresh"
C2SROOMSTARTREFRESH.full_name = ".net_protocol.C2SRoomStartRefresh"
C2SROOMSTARTREFRESH.nested_types = {}
C2SROOMSTARTREFRESH.enum_types = {}
C2SROOMSTARTREFRESH.fields = {}
C2SROOMSTARTREFRESH.is_extendable = false
C2SROOMSTARTREFRESH.extensions = {}
C2SROOMEXITROOM.name = "C2SRoomExitRoom"
C2SROOMEXITROOM.full_name = ".net_protocol.C2SRoomExitRoom"
C2SROOMEXITROOM.nested_types = {}
C2SROOMEXITROOM.enum_types = {}
C2SROOMEXITROOM.fields = {}
C2SROOMEXITROOM.is_extendable = false
C2SROOMEXITROOM.extensions = {}
C2SROOMJJJKKK_ADJHASJKD_FIELD.name = "adjhasjkd"
C2SROOMJJJKKK_ADJHASJKD_FIELD.full_name = ".net_protocol.C2SRoomJjjKkk.adjhasjkd"
C2SROOMJJJKKK_ADJHASJKD_FIELD.number = 1
C2SROOMJJJKKK_ADJHASJKD_FIELD.index = 0
C2SROOMJJJKKK_ADJHASJKD_FIELD.label = 2
C2SROOMJJJKKK_ADJHASJKD_FIELD.has_default_value = false
C2SROOMJJJKKK_ADJHASJKD_FIELD.default_value = ""
C2SROOMJJJKKK_ADJHASJKD_FIELD.type = 9
C2SROOMJJJKKK_ADJHASJKD_FIELD.cpp_type = 9

C2SROOMJJJKKK_JALDAJLAS_FIELD.name = "jaldajlas"
C2SROOMJJJKKK_JALDAJLAS_FIELD.full_name = ".net_protocol.C2SRoomJjjKkk.jaldajlas"
C2SROOMJJJKKK_JALDAJLAS_FIELD.number = 2
C2SROOMJJJKKK_JALDAJLAS_FIELD.index = 1
C2SROOMJJJKKK_JALDAJLAS_FIELD.label = 2
C2SROOMJJJKKK_JALDAJLAS_FIELD.has_default_value = false
C2SROOMJJJKKK_JALDAJLAS_FIELD.default_value = ""
C2SROOMJJJKKK_JALDAJLAS_FIELD.type = 9
C2SROOMJJJKKK_JALDAJLAS_FIELD.cpp_type = 9

C2SROOMJJJKKK_DASJDKAHD_FIELD.name = "dasjdkahd"
C2SROOMJJJKKK_DASJDKAHD_FIELD.full_name = ".net_protocol.C2SRoomJjjKkk.dasjdkahd"
C2SROOMJJJKKK_DASJDKAHD_FIELD.number = 3
C2SROOMJJJKKK_DASJDKAHD_FIELD.index = 2
C2SROOMJJJKKK_DASJDKAHD_FIELD.label = 2
C2SROOMJJJKKK_DASJDKAHD_FIELD.has_default_value = false
C2SROOMJJJKKK_DASJDKAHD_FIELD.default_value = ""
C2SROOMJJJKKK_DASJDKAHD_FIELD.type = 9
C2SROOMJJJKKK_DASJDKAHD_FIELD.cpp_type = 9

C2SROOMJJJKKK.name = "C2SRoomJjjKkk"
C2SROOMJJJKKK.full_name = ".net_protocol.C2SRoomJjjKkk"
C2SROOMJJJKKK.nested_types = {}
C2SROOMJJJKKK.enum_types = {}
C2SROOMJJJKKK.fields = {C2SROOMJJJKKK_ADJHASJKD_FIELD, C2SROOMJJJKKK_JALDAJLAS_FIELD, C2SROOMJJJKKK_DASJDKAHD_FIELD}
C2SROOMJJJKKK.is_extendable = false
C2SROOMJJJKKK.extensions = {}

C2SRoomEnterRoom = protobuf.Message(C2SROOMENTERROOM)
C2SRoomExitRoom = protobuf.Message(C2SROOMEXITROOM)
C2SRoomJjjKkk = protobuf.Message(C2SROOMJJJKKK)
C2SRoomStartRefresh = protobuf.Message(C2SROOMSTARTREFRESH)


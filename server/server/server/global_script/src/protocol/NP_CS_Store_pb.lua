-- Generated By protoc-gen-lua Do not Edit
local protobuf = require "protobuf"
module('NP_CS_Store_pb')


C2SSTOREBUY = protobuf.Descriptor();
local C2SSTOREBUY_ID_FIELD = protobuf.FieldDescriptor();

C2SSTOREBUY_ID_FIELD.name = "id"
C2SSTOREBUY_ID_FIELD.full_name = ".net_protocol.C2SStoreBuy.id"
C2SSTOREBUY_ID_FIELD.number = 1
C2SSTOREBUY_ID_FIELD.index = 0
C2SSTOREBUY_ID_FIELD.label = 2
C2SSTOREBUY_ID_FIELD.has_default_value = false
C2SSTOREBUY_ID_FIELD.default_value = 0
C2SSTOREBUY_ID_FIELD.type = 13
C2SSTOREBUY_ID_FIELD.cpp_type = 3

C2SSTOREBUY.name = "C2SStoreBuy"
C2SSTOREBUY.full_name = ".net_protocol.C2SStoreBuy"
C2SSTOREBUY.nested_types = {}
C2SSTOREBUY.enum_types = {}
C2SSTOREBUY.fields = {C2SSTOREBUY_ID_FIELD}
C2SSTOREBUY.is_extendable = false
C2SSTOREBUY.extensions = {}

C2SStoreBuy = protobuf.Message(C2SSTOREBUY)

-- Generated By protoc-gen-lua Do not Edit
local protobuf = require "protobuf"
module('gl2ga_pb')


GL2GAREQROLEENTERSCENE = protobuf.Descriptor();
local GL2GAREQROLEENTERSCENE_NET_ID_FIELD = protobuf.FieldDescriptor();
local GL2GAREQROLEENTERSCENE_GW_SRV_ID_FIELD = protobuf.FieldDescriptor();
GL2GAKICKROLE = protobuf.Descriptor();
local GL2GAKICKROLE_NET_ID_FIELD = protobuf.FieldDescriptor();

GL2GAREQROLEENTERSCENE_NET_ID_FIELD.name = "net_id"
GL2GAREQROLEENTERSCENE_NET_ID_FIELD.full_name = ".net_protocol.GL2GAReqRoleEnterScene.net_id"
GL2GAREQROLEENTERSCENE_NET_ID_FIELD.number = 1
GL2GAREQROLEENTERSCENE_NET_ID_FIELD.index = 0
GL2GAREQROLEENTERSCENE_NET_ID_FIELD.label = 2
GL2GAREQROLEENTERSCENE_NET_ID_FIELD.has_default_value = false
GL2GAREQROLEENTERSCENE_NET_ID_FIELD.default_value = 0
GL2GAREQROLEENTERSCENE_NET_ID_FIELD.type = 13
GL2GAREQROLEENTERSCENE_NET_ID_FIELD.cpp_type = 3

GL2GAREQROLEENTERSCENE_GW_SRV_ID_FIELD.name = "gw_srv_id"
GL2GAREQROLEENTERSCENE_GW_SRV_ID_FIELD.full_name = ".net_protocol.GL2GAReqRoleEnterScene.gw_srv_id"
GL2GAREQROLEENTERSCENE_GW_SRV_ID_FIELD.number = 2
GL2GAREQROLEENTERSCENE_GW_SRV_ID_FIELD.index = 1
GL2GAREQROLEENTERSCENE_GW_SRV_ID_FIELD.label = 2
GL2GAREQROLEENTERSCENE_GW_SRV_ID_FIELD.has_default_value = false
GL2GAREQROLEENTERSCENE_GW_SRV_ID_FIELD.default_value = 0
GL2GAREQROLEENTERSCENE_GW_SRV_ID_FIELD.type = 13
GL2GAREQROLEENTERSCENE_GW_SRV_ID_FIELD.cpp_type = 3

GL2GAREQROLEENTERSCENE.name = "GL2GAReqRoleEnterScene"
GL2GAREQROLEENTERSCENE.full_name = ".net_protocol.GL2GAReqRoleEnterScene"
GL2GAREQROLEENTERSCENE.nested_types = {}
GL2GAREQROLEENTERSCENE.enum_types = {}
GL2GAREQROLEENTERSCENE.fields = {GL2GAREQROLEENTERSCENE_NET_ID_FIELD, GL2GAREQROLEENTERSCENE_GW_SRV_ID_FIELD}
GL2GAREQROLEENTERSCENE.is_extendable = false
GL2GAREQROLEENTERSCENE.extensions = {}
GL2GAKICKROLE_NET_ID_FIELD.name = "net_id"
GL2GAKICKROLE_NET_ID_FIELD.full_name = ".net_protocol.GL2GAKickRole.net_id"
GL2GAKICKROLE_NET_ID_FIELD.number = 1
GL2GAKICKROLE_NET_ID_FIELD.index = 0
GL2GAKICKROLE_NET_ID_FIELD.label = 2
GL2GAKICKROLE_NET_ID_FIELD.has_default_value = false
GL2GAKICKROLE_NET_ID_FIELD.default_value = 0
GL2GAKICKROLE_NET_ID_FIELD.type = 13
GL2GAKICKROLE_NET_ID_FIELD.cpp_type = 3

GL2GAKICKROLE.name = "GL2GAKickRole"
GL2GAKICKROLE.full_name = ".net_protocol.GL2GAKickRole"
GL2GAKICKROLE.nested_types = {}
GL2GAKICKROLE.enum_types = {}
GL2GAKICKROLE.fields = {GL2GAKICKROLE_NET_ID_FIELD}
GL2GAKICKROLE.is_extendable = false
GL2GAKICKROLE.extensions = {}

GL2GAKickRole = protobuf.Message(GL2GAKICKROLE)
GL2GAReqRoleEnterScene = protobuf.Message(GL2GAREQROLEENTERSCENE)


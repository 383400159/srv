/*
** Lua binding: server
** Generated automatically by tolua++-1.0.92 on 08/13/17 18:24:43.
*/

#ifndef __cplusplus
#include "stdlib.h"
#endif
#include "string.h"

#include "tolua++.h"

/* Exported function */
TOLUA_API int  tolua_server_open (lua_State* tolua_S);

#include "../interface/lua_server_export.h"

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
}

/* function: CppSrv::DebugLog */
#ifndef TOLUA_DISABLE_tolua_server_CppSrv_DebugLog00
static int tolua_server_CppSrv_DebugLog00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_iscppstring(tolua_S,1,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  std::string log = ((std::string)  tolua_tocppstring(tolua_S,1,0));
  {
   CppSrv::DebugLog(log);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'DebugLog'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_server_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_module(tolua_S,"CppSrv",0);
  tolua_beginmodule(tolua_S,"CppSrv");
   tolua_function(tolua_S,"DebugLog",tolua_server_CppSrv_DebugLog00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_server (lua_State* tolua_S) {
 return tolua_server_open(tolua_S);
};
#endif


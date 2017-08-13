#include <iostream>
#include "INetworkASIO.h"
#include "mysqlmanager.h"
#include "MyMessage.pb.h"
#include "scriptmanager.hpp"
using namespace std;

#include "lua_call_cpp/interface/lua_server_export.h"
//lua ͷ�ļ�
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};


/*
int main()
{
	//auto mysql = new mysqlmanager();
	mysqlSrv.start();
	
	mysqlSrv.write_data_by_two_key(1,"testkey","testkey1","testVAL666 TEST");
	//mysql->read(1,"select * from test");
	//mysql->read(2,"select * from test1");
	//mysql->write(1,"update test set degree=88.8");
	//mysql->write(2,"update test1 set degree=888");
	//const char * sql = "update test set degree=888 ";
	

	
	
	//mysql->write();
	getchar(); 
	return 0;
}
*/
void testSimpleMessage()
{
	printf("==================This is simple message.================\n");
	//���л�LogonReqMessage����ָ�����ڴ�����
	net_protocol::LogonReqMessage logonReq;
	logonReq.set_acctid(20);
	logonReq.set_passwd("Hello World");
	//��ǰ��ȡ�������л���ռ�õĿռ䲢����һ���Է��䣬�Ӷ������η���
	//����ɵ����ܿ�����ͨ�����ַ�ʽ�������Խ����л�������ݽ��м��ܡ�
	//֮���ٽ��г־û������Ƿ��͵�Զ�ˡ�
	int length = logonReq.ByteSize();
	char* buf = new char[length];
	logonReq.SerializeToArray(buf,length);

	//���ڴ��ж�ȡ�������л�LogonReqMessage����ͬʱ�������ӡ������
	net_protocol::LogonReqMessage logonReq2;
	logonReq2.ParseFromArray(buf,length);
	printf("acctID = %I64d, password = %s\n",logonReq2.acctid(),logonReq2.passwd().c_str());
	delete [] buf;
}


void call_lua(){

	//��Lua
	lua_State *L = luaL_newstate();
	/*����lua���п�*/
	luaL_openlibs(L);

	/*����test.lua�ļ�������*/
	int bRet = luaL_dofile(L, "../global_script/premain.lua");
	if (bRet)
	{
		cout << "load file error" << endl;
		return;
	}
 
	lua_getglobal(L, "add_path_to_env");        // ��ȡ������ѹ��ջ��  
	lua_pushstring(L, "../global_script/?.lua;");          // ѹ�����  
	int iRet = lua_pcall(L, 1, 1, 0);// ���ú�������������Ժ󣬻Ὣ����ֵѹ��ջ�У���һ��1��ʾ�����������ڶ���1��ʾ���ؽ�������� 

	lua_getglobal(L, "add_path_to_env");        // ��ȡ������ѹ��ջ��  
	lua_pushstring(L, "../global_script/src/?.lua;");          // ѹ�����  
	iRet = lua_pcall(L, 1, 1, 0);// ���ú�������������Ժ󣬻Ὣ����ֵѹ��ջ�У���һ��1��ʾ�����������ڶ���1��ʾ���ؽ��������

	lua_getglobal(L, "add_path_to_env");        // ��ȡ������ѹ��ջ��  
	lua_pushstring(L, "../global_script/src/protocol/?.lua;");          // ѹ�����  
	iRet = lua_pcall(L, 1, 1, 0);// ���ú�������������Ժ󣬻Ὣ����ֵѹ��ջ�У���һ��1��ʾ�����������ڶ���1��ʾ���ؽ��������

	lua_getglobal(L, "add_path_to_env");        // ��ȡ������ѹ��ջ��  
	lua_pushstring(L, "../global_script/src/db_param/?.lua;");          // ѹ�����  
	iRet = lua_pcall(L, 1, 1, 0);// ���ú�������������Ժ󣬻Ὣ����ֵѹ��ջ�У���һ��1��ʾ�����������ڶ���1��ʾ���ؽ��������

	int cRet = luaL_dofile(L, "../global_script/main.lua");
	if (cRet)
	{
		cout << "load file error" << endl;
		return;
	}
	
	int dRet = luaL_dofile(L, "../global_script/src/server.lua");
	if (dRet)                       // ���ó���  
	{
		const char *pErrorMsg = lua_tostring(L, -1);
		cout << pErrorMsg << endl;
		lua_close(L);
		return;
	}

	/*���lua*/
	lua_close(L);
	getchar();
}

void main()
{
	//call_lua();
	std::string script_dir = "../game_script";
	std::string data_dir = "../data";
	std::string error;
	if (!sScriptManager.Init(script_dir, data_dir, &error)){
		std::cout<<"start lua state fail!!!!!!!!!!!!!!!!!"<<std::endl;
	}	
	int i = 0 ;
	while (true)
	{
		sScriptManager.PCall("update", LARG_END);
		i++;
		if (i==100)
		{
			sScriptManager.ReloadAllScripts();
		}
		if (i==103)
		{
			break;
		}
	}

	getchar(); 
}

/*
int main(int argc, char *argv[])
{

	std::cout<<"start--------------------------------"<<std::endl;
	//����������֮�����Ϣ
	INetworkASIO* server_net_work = new INetworkASIO(2);
	server_net_work->Listen(7001,5);

	//�����ͻ�����Ϣ
	INetworkASIO* client_net_work = new INetworkASIO(8);
	client_net_work->Listen(19001,5);

	std::cout<<"test"<<std::endl;

	getchar(); 
	return 0;
}
*/


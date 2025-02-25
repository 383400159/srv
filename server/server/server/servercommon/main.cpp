#include <iostream>
#include "ServerMgr.h"
#include "INetworkASIO.h"
#include "IMysqlDB.h"
#include "MyMessage.pb.h"
#include "scriptmanager.hpp"
using namespace std;

#include "lua_call_cpp/interface/lua_server_export.h"
//lua 头文件
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
};



//int main()
//{
//	auto mysql = new IMysqlDB();
//	mysql->Start();
//	
//	mysql->write_data_by_two_key(1,"testkey","testkey1","testVAL666 TEST");
//	//mysql->read(1,"select * from test");
//	//mysql->read(2,"select * from test1");
//	//mysql->write(1,"update test set degree=88.8");
//	//mysql->write(2,"update test1 set degree=888");
//	//const char * sql = "update test set degree=888 ";
//	
//
//	
//	
//	//mysql->write();
//	getchar(); 
//	return 0;
//}

void testSimpleMessage()
{
	printf("==================This is simple message.================\n");
	//序列化LogonReqMessage对象到指定的内存区域。
	net_protocol::LogonReqMessage logonReq;
	logonReq.set_acctid(20);
	logonReq.set_passwd("Hello World");
	//提前获取对象序列化所占用的空间并进行一次性分配，从而避免多次分配
	//而造成的性能开销。通过该种方式，还可以将序列化后的数据进行加密。
	//之后再进行持久化，或是发送到远端。
	int length = logonReq.ByteSize();
	char* buf = new char[length];
	logonReq.SerializeToArray(buf,length);

	//从内存中读取并反序列化LogonReqMessage对象，同时将结果打印出来。
	net_protocol::LogonReqMessage logonReq2;
	logonReq2.ParseFromArray(buf,length);
	printf("acctID = %I64d, password = %s\n",logonReq2.acctid(),logonReq2.passwd().c_str());
	delete [] buf;
}


void call_lua(){

	//打开Lua
	lua_State *L = luaL_newstate();
	/*加载lua所有库*/
	luaL_openlibs(L);

	/*加载test.lua文件并运行*/
	int bRet = luaL_dofile(L, "../global_script/premain.lua");
	if (bRet)
	{
		cout << "load file error" << endl;
		return;
	}
 
	lua_getglobal(L, "add_path_to_env");        // 获取函数，压入栈中  
	lua_pushstring(L, "../global_script/?.lua;");          // 压入参数  
	int iRet = lua_pcall(L, 1, 1, 0);// 调用函数，调用完成以后，会将返回值压入栈中，第一个1表示参数个数，第二个1表示返回结果个数。 

	lua_getglobal(L, "add_path_to_env");        // 获取函数，压入栈中  
	lua_pushstring(L, "../global_script/src/?.lua;");          // 压入参数  
	iRet = lua_pcall(L, 1, 1, 0);// 调用函数，调用完成以后，会将返回值压入栈中，第一个1表示参数个数，第二个1表示返回结果个数。

	lua_getglobal(L, "add_path_to_env");        // 获取函数，压入栈中  
	lua_pushstring(L, "../global_script/src/protocol/?.lua;");          // 压入参数  
	iRet = lua_pcall(L, 1, 1, 0);// 调用函数，调用完成以后，会将返回值压入栈中，第一个1表示参数个数，第二个1表示返回结果个数。

	lua_getglobal(L, "add_path_to_env");        // 获取函数，压入栈中  
	lua_pushstring(L, "../global_script/src/db_param/?.lua;");          // 压入参数  
	iRet = lua_pcall(L, 1, 1, 0);// 调用函数，调用完成以后，会将返回值压入栈中，第一个1表示参数个数，第二个1表示返回结果个数。

	int cRet = luaL_dofile(L, "../global_script/main.lua");
	if (cRet)
	{
		cout << "load file error" << endl;
		return;
	}
	
	int dRet = luaL_dofile(L, "../global_script/src/server.lua");
	if (dRet)                       // 调用出错  
	{
		const char *pErrorMsg = lua_tostring(L, -1);
		cout << pErrorMsg << endl;
		lua_close(L);
		return;
	}

	/*清除lua*/
	lua_close(L);
	getchar();
}


//lua 启动和热更
//
//void main()
//{
//	//call_lua();
//	std::string script_dir = "../game_script";
//	std::string data_dir = "../data";
//	std::string error;
//	if (!sScriptManager.Init(script_dir, data_dir, &error)){
//		std::cout<<"start lua state fail!!!!!!!!!!!!!!!!!"<<std::endl;
//	}	
//	int i = 0 ;
//	while (true)
//	{
//		std::cout<<"按下任意键开始热更"<<std::endl;
//		getchar(); 
//		sScriptManager.ReloadAllScripts();
//		sScriptManager.PCall("update", LARG_END);
//		/*sScriptManager.PCall("update", LARG_END);
//		i++;
//		if (i==100)
//		{
//			sScriptManager.ReloadAllScripts();
//		}
//		if (i==103)
//		{
//			break;
//		}*/
//	}
//
//	getchar(); 
//}


//
//int main(int argc, char *argv[])
//{
//
//	std::cout<<"start--------------------------------"<<std::endl;
//	//监听服务器之间的消息
//	INetworkASIO* server_net_work = new INetworkASIO(2);
//	server_net_work->Listen(7002,5);
//
//	//监听客户端消息
//	INetworkASIO* client_net_work = new INetworkASIO(8);
//	client_net_work->Listen(19002,5);
//
//	IMysqlDB* sql_mgr = new IMysqlDB();
//
//
//	ServerMgr * server_mgr = new ServerMgr();
//	//向服务器注册节点
//	server_mgr->RegisterModule("ServerINetworkASIO",server_net_work);
//	server_mgr->RegisterModule("ClientINetworkASIO",client_net_work);
//	server_mgr->RegisterModule("IMysqlDB",sql_mgr);
//
//	//auto asio1 = dynamic_cast<INetworkASIO *>(server_mgr->QueryModule("INetworkASIO"));
//	//auto asio2 = asio1->Interface()->QueryModule("INetworkASIO");
//	//asio1->Listen(7002,5);
//	getchar(); 
//	delete server_net_work;
//	delete client_net_work;
//	delete server_mgr;
//	return 0;
//}

//lambda语法
//[]  不捕获任何变量
//[&] 以引用方式捕获所有变量
//[=] 用值的方式捕获所有变量（可能被编译器优化为const &)
//[=, &foo] 以引用捕获foo, 但其余变量都靠值捕获
//[bar] 以值方式捕获bar; 不捕获其它变量
//[this] 捕获所在类的this指针
//int main(int argc,char *argv[])
//{
//	[](){std::cout<<"hello lambda"<<std::endl;}();
//
//	auto a = [](){return 1;}();
//	std::cout<<a<<std::endl;
//
//	char bb='a';
//	int b = [&]()->int{return bb;}();
//	std::cout<<b<<std::endl;
//
//	[]()throw(){}();
//
//	//std::function<int ()> func;
//	//// 检测是否包含函数
//	//if ( func )
//	//{
//	//	// if we did have a function, call it
//	//	func();
//	//}
//
//	
//	auto f = [] () -> int { return 2; };
//	f();
//
//	//区间迭代
//	vector<int> vec;
//	vec.push_back( 10 );
//	vec.push_back( 20 );
//
//	for (auto i : vec )
//	{
//		cout << i;
//	}
//	
//	//vs2012不支持constexpr
//	//constexpr int getDefaultArraySize (int multiplier)
//	//{
//	//	return 10 * multiplier;
//	//}
//
//	//int my_array[ getDefaultArraySize( 3 ) ];
//
//	getchar(); 
//}


//模板测试
int main(int argc,char *argv[])
{

}
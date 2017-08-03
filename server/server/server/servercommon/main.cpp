#include <iostream>
#include "INetworkModule.h"
#include "mysqlmanager.h"
using namespace std;
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


int main(int argc, char *argv[])
{

	std::cout<<"start--------------------------------"<<std::endl;
	//监听服务器之间的消息
	INetworkModule* server_net_work = new INetworkModule(2);
	server_net_work->Listen(7001,5);

	//监听客户端消息
	INetworkModule* client_net_work = new INetworkModule(8);
	client_net_work->Listen(19001,5);

	std::cout<<"test"<<std::endl;
	getchar(); 
	return 0;
}



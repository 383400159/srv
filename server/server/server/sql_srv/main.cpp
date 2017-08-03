#include <iostream>
#include "INetworkASIO.h"
#include "mysqlmanager.h"
using namespace std;

int main(int argc, char *argv[])
{

	std::cout<<"start--------------------------------"<<std::endl;
	//监听服务器之间的消息
	INetworkASIO* server_net_work = new INetworkASIO(2);
	server_net_work->Listen(7001,5);

	std::cout<<"sql-srv start end--------------------"<<std::endl;
	getchar(); 
	return 0;
}



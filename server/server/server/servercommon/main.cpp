#include <iostream>
#include "INetworkModule.h"




int main(int argc, char *argv[])
{

	std::cout<<"start--------------------------------"<<std::endl;
	//监听服务器之间的消息
	INetworkModule* server_net_work = new INetworkModule();
	server_net_work->Listen(7001,5);

	//监听客户端消息
	INetworkModule* client_net_work = new INetworkModule();
	client_net_work->Listen(19001,5);

	std::cout<<"test"<<std::endl;
	getchar(); 
	return 0;
}



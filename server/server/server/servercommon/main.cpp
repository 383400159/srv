#include <iostream>
#include "INetworkModule.h"




int main(int argc, char *argv[])
{

	std::cout<<"start--------------------------------"<<std::endl;
	//����������֮�����Ϣ
	INetworkModule* server_net_work = new INetworkModule();
	server_net_work->Listen(7001,5);

	//�����ͻ�����Ϣ
	INetworkModule* client_net_work = new INetworkModule();
	client_net_work->Listen(19001,5);

	std::cout<<"test"<<std::endl;
	getchar(); 
	return 0;
}



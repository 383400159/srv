#include <iostream>
#include "INetworkModule.h"
#include "MysqlModule.h"
using namespace std;
int main()
{
	auto mysql = new MysqlModule();
	mysql->start();
	return 0;
}


/*
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
*/


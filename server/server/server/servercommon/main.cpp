#include <iostream>
#include "INetworkModule.h"




int main(int argc, char *argv[])
{

	std::cout<<"start--------------------------------"<<std::endl;
	INetworkModule* net_work = new INetworkModule();
	net_work->Listen(19001,5);

	std::cout<<"test"<<std::endl;
	getchar(); 
	return 0;
}



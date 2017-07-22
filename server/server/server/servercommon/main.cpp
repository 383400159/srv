#include <iostream>
#include "INetworkModule.h"

int main(int argc, char *argv[])
{
	std::cout<<"start--------------------------------"<<std::endl;
	INetworkModule* net_work = new INetworkModule();
	Port ip = 
	net_work->Listen(19001,5);

	while (true)
	{
		net_work->Update();
	}
	return 0;
}


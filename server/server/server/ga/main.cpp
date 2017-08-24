#include <iostream>
#include "INetworkASIO.h"




int main(int argc, char *argv[])
{

	std::cout<<"start--------------------------------"<<std::endl;
	INetworkASIO* net_work = new INetworkASIO();
	
	net_work->Connect("192.168.1.136",7002,1,3000);
	std::cout<<"test"<<std::endl;
	getchar(); 
	return 0;
}



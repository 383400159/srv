#include <iostream>
#include "INetworkModule.h"


#include <stdio.h> 
#include <cstdlib> 
#include <iostream> 
#include <boost/thread.hpp> 
#include <boost/aligned_storage.hpp> 
#include <boost/array.hpp> 
#include <boost/bind.hpp> 
#include <boost/enable_shared_from_this.hpp> 
#include <boost/noncopyable.hpp> 
#include <boost/shared_ptr.hpp> 
#include <boost/asio.hpp> 
using namespace std; // For atoi. 
int role_num = 0;
void handle_accept()
{
	role_num ++ ;
   cout<<" handle_accept:"<<role_num<<endl;
}

void start()
{
	io_service iosev;
	ip::tcp::socket socket_(iosev); 
	// 所有asio类都需要io_service对象
	ip::tcp::acceptor acceptor_(iosev, ip::tcp::endpoint(ip::tcp::v4(), 19001));
	acceptor_.async_accept(socket_,boost::bind(&handle_accept));
	iosev.run();
}

int main(int argc , char* argv[])
{
	boost::thread thrd(&start);

	getchar(); 
	thrd.join();

	return 0;
}



/*

int main(int argc, char *argv[])
{

	std::cout<<"start--------------------------------"<<std::endl;
	INetworkModule* net_work = new INetworkModule();
	net_work->Listen(19001,5);

	while (true)
	{
		net_work->Update();
	}
	return 0;
}

*/

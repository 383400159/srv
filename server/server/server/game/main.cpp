#ifdef _MSC_VER
#define _WIN32_WINNT 0x0501
#endif
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp> 
#include <iostream>

using namespace std;
using namespace boost::asio;
boost::asio::io_service m_io;
boost::asio::ip::tcp::acceptor* m_acceptor;
void start();
void accept_handler(const boost::system::error_code& ec, boost::shared_ptr<ip::tcp::socket> sock)
{
	if (ec)
	{    return;    }

	cout<<"Client:";
	cout<<sock->remote_endpoint().address()<<endl;
	m_acceptor->async_accept(*sock, boost::bind(&accept_handler, boost::asio::placeholders::error, sock));
	// 发送完毕后继续监听，否则io_service将认为没有事件处理而结束运行
	start();
}

void start(){
	//新建一个socket 用来等待玩家进来
	boost::shared_ptr<ip::tcp::socket> sock(new ip::tcp::socket(m_io));
	m_acceptor->async_accept(*sock, boost::bind(&accept_handler, boost::asio::placeholders::error, sock));
}

void run(){
	m_io.run();
}
int main(int argc, char *argv[])
{
	//初始化控制器
	m_acceptor = new ip::tcp::acceptor(m_io,ip::tcp::endpoint(ip::tcp::v4(), 19001));

	start();
	boost::thread thrd(&run);
	std::cout<<"test"<<std::endl;
	getchar(); 

	return 0;
}


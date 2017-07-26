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


//using namespace boost::asio;


class server
{

private:
	boost::asio::io_service m_io;
	ip::tcp::acceptor m_acceptor;

public:
	server() : m_acceptor(m_io, ip::tcp::endpoint(ip::tcp::v4(), 19001))
	{    
		accept();    
	}

	void run(){ m_io.run();}

	void accept()
	{
		boost::shared_ptr<ip::tcp::socket> sock(new ip::tcp::socket(m_io));
		m_acceptor.async_accept(*sock, boost::bind(&server::accept_handler, this, boost::asio::placeholders::error, sock));
	}

	void accept_handler(const boost::system::error_code& ec, boost::shared_ptr<ip::tcp::socket> sock)
	{
		if (ec)
		{    return;    }

		cout<<"Client:";
		cout<<sock->remote_endpoint().address()<<endl;
		sock->async_write_some(buffer("hello asio"), boost::bind(&server::write_handler, this, boost::asio::placeholders::error));
		// 发送完毕后继续监听，否则io_service将认为没有事件处理而结束运行
		accept();
	}

	void write_handler(const boost::system::error_code&ec)
	{
		cout<<"send msg complete"<<endl;
	}
};

int main()
{
	try
	{
		cout<<"Server start."<<endl;
		server srv;
		srv.run();
	}
	catch (std::exception &e)
	{
		cout<<e.what()<<endl;
	}

	return 0;
}



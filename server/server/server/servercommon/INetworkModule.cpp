#include "INetworkModule.h"




INetworkModule::INetworkModule(void)
{

}


INetworkModule::~INetworkModule(void)
{

}



int INetworkModule::Init()
{

	return 0;
}

/*
	创建监听句柄
	@port			要监听的端口号
	@backlog		所允许的半开连接的最大个数
	@netid_out		out参数，监听成功则该参数为监听句柄的netid。注意，销毁监听句柄时使用Disconnect函数，并将该netid作为传入参数
	@ip_bind		listen 时的绑定socket 的 ip，如果为0则绑定到INADDR_ANY
	@Return		是否监听成功
*/
bool INetworkModule::Listen(Port port, int backlog, NetID *netid_out, const char *ip_bind)
{

	// 所有asio类都需要io_service对象
	acceptor = new ip::tcp::acceptor(iosev, ip::tcp::endpoint(ip::tcp::v4(), port));
	
	return true;
}

	/*
	 建立网络连接
	 @ip			要连接的远端地址
	 @port			要连接的远端端口号
	 @netid_out		out参数，连接成功时netid_out为建立连接的网络id号
	 @time_out		建立网络连接动作的超时时间，单位毫秒，默认为3000
	 @Return		是否连接成功
	*/
bool INetworkModule::Connect(const char *ip, Port port, NetID *netid_out, unsigned long time_out)
{
	// socket对象
	ip::tcp::socket socket(iosev);
	// 连接端点，这里使用了本机连接，可以修改IP地址测试远程连接
	ip::tcp::endpoint ep(ip::address_v4::from_string(ip), port);
	// 连接服务器
	boost::system::error_code ec;
	socket.connect(ep,ec);
	// 如果出错，打印出错信息
	if(ec)
	{
		std::cout << boost::system::system_error(ec).what() << std::endl;
		return false;
	}
	// 接收数据
	char buf[100];
	size_t len=socket.read_some(buffer(buf), ec);
	std::cout.write(buf, len);
	return true;
}

/*
	发送消息包，注意，该函数并不代表数据已经成功发送，而是将消息放到待发送队列中异步发送
	如果某个连接断开回调disconnect时，有可能导致消息包丢失
	该函数对消息包发送顺序是有保证的
	@neiid			发送目标的网络id
	@data			发送的消息包
	@length		消息包长度
	@Return		是否成功放到待发送队列中
*/
bool INetworkModule::Send(NetID netid, const char *data, unsigned int length)
{
	
	return true;
}





int INetworkModule::Update()
{
	ip::tcp::socket socket(iosev);
	// 等待直到客户端连接进来
	acceptor->accept(socket);
	// 显示连接进来的客户端
	std::cout << socket.remote_endpoint().address()<<" "<<socket.remote_endpoint().port()<< std::endl;
	// 向客户端发送hello world!
	boost::system::error_code ec;
	socket.write_some(buffer("hello world!"), ec);

	// 如果出错，打印出错信息
	if(ec)
	{
		std::cout << 
			boost::system::system_error(ec).what() << std::endl;
	}

	return 0;
}

void IEngineNetCallback::OnAccept(Port listen_port, NetID netid, IP ip, Port port)
{
	std::cout<<" listen_port:"<<listen_port<<" netid:"<<netid<<" ip"<<ip<<" port"<<port<<std::endl;
}

void IEngineNetCallback::OnRecv(NetID netid, const char *data, int length)
{
	std::cout<<"netid:"<<netid<<" data:"<<*data<<" length:"<<length<<std::endl;
}

IEngineNetCallback::IEngineNetCallback()
{

}

IEngineNetCallback::~IEngineNetCallback()
{

}
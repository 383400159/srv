#include "INetworkASIO.h"




INetworkASIO::INetworkASIO(std::size_t io_service_pool_size)
	: io_service_pool_(io_service_pool_size) 
	, io_service_work_pool_(io_service_pool_size) 
{
	this->Init();
}

INetworkASIO::INetworkASIO()
	: io_service_pool_(1) 
	, io_service_work_pool_(1) 
{
	this->Init();
}

INetworkASIO::~INetworkASIO(void)
{
	this->Stop();
}



int INetworkASIO::Init()
{
	NetIDCount_ = 0;
	callback_ = new ASIOCallback();
	return 0;
}

void INetworkASIO::Run()
{
	io_service_.run();
}
int INetworkASIO::Stop()
{
	io_service_.stop();
	io_thread_->join();
	obj_io_thread_->join();
	obj_work_thread_->join();

	return 1;
}
int INetworkASIO::Start()
{
	//一个session代表一个玩家
	session_ptr new_session( new INetworkSession(io_service_pool_.get_io_service(),io_service_work_pool_.get_io_service()));
	acceptor_->async_accept(new_session->socket(), boost::bind(&INetworkASIO::handle_accept,this,new_session,boost::asio::placeholders::error));
	return 1;
}
/*
	创建监听句柄
	@port			要监听的端口号
	@backlog		所允许的半开连接的最大个数
	@netid_out		out参数，监听成功则该参数为监听句柄的netid。注意，销毁监听句柄时使用Disconnect函数，并将该netid作为传入参数
	@ip_bind		listen 时的绑定socket 的 ip，如果为0则绑定到INADDR_ANY
	@Return		是否监听成功
*/
bool INetworkASIO::Listen(Port port, int backlog, NetID netid_out, const char *ip_bind)
{
	// 所有asio类都需要io_service对象
	acceptor_ = new ip::tcp::acceptor(io_service_, ip::tcp::endpoint(ip::tcp::v4(), port));
	this->Start();
	//因为io_service.run会堵塞线程 所以用一个线程来防堵塞
	io_thread_.reset(new boost::thread(boost::bind(&INetworkASIO::Run, this))); 
	obj_io_thread_.reset(new boost::thread(boost::bind(&io_service_pool::run, io_service_pool_)));
	obj_work_thread_.reset(new boost::thread(boost::bind(&io_service_pool::run, io_service_work_pool_)));
	return true;
}

 void INetworkASIO::handle_accept(session_ptr callback_session, const boost::system::error_code& error)
 {
	 if (!error) 
	 { 
		 //给玩家分配一个netid 代表唯一id
		 NetIDCount_++;
		 callback_session->set_netid(NetIDCount_);
		 callback_session->RegisterCallback(callback_);
		 callback_session->start();
		 NetIDList_.insert(std::make_pair(NetIDCount_,callback_session));
		 callback_session->write("hello my is server");
		 std::cout<<"Client:";
		 std::cout<<callback_session->socket().remote_endpoint().address()<<std::endl;
		 // 发送完毕后继续监听，否则io_service将认为没有事件处理而结束运行
	 }
	 else
	 {
		 std::cout <<"handle_accept error:" <<error.value();
		 //throw boost::system::system_error(error);  
	 }
	 //无论如何都要继续监听
	 Start();
 }
	/*
	 建立网络连接
	 @ip			要连接的远端地址
	 @port			要连接的远端端口号
	 @netid_out		out参数，连接成功时netid_out为建立连接的网络id号
	 @time_out		建立网络连接动作的超时时间，单位毫秒，默认为3000
	 @Return		是否连接成功
	*/
bool INetworkASIO::Connect(const char *ip, Port port, NetID netid_out, unsigned long time_out)
{
	session_ptr new_session( new INetworkSession(io_service_,io_service_));
	// 连接端点，这里使用了本机连接，可以修改IP地址测试远程连接
	ip::tcp::endpoint ep(ip::address_v4::from_string(ip), port);
	// 连接服务器
	boost::system::error_code ec;
	new_session->socket().async_connect(ep, boost::bind(&INetworkASIO::handle_connect,this,new_session,netid_out,boost::asio::placeholders::error));
	//因为io_service.run会堵塞线程 所以用一个线程来防堵塞
	io_thread_.reset(new boost::thread(boost::bind(&INetworkASIO::Run, this))); 
	return true;
}

  /*
	 异步ping回调
	 */
void INetworkASIO::handle_connect(session_ptr callback_session,NetID netid_out,const boost::system::error_code& error)
{
	if (!error)
	{
		callback_session->set_netid(netid_out);
		callback_session->RegisterCallback(callback_);
		callback_session->start();
		//ServerList_.insert(std::make_pair(netid_out,callback_session));
		NetIDList_.insert(std::make_pair(netid_out,callback_session));
		callback_session->write("hello my is client");
	}
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
bool INetworkASIO::Send(NetID netid, const char *data, unsigned int length)
{
	auto it = NetIDList_.find(netid);
	if (it!=NetIDList_.end())
	{
		//目标对象存在可以发送消息
		session_ptr callback_session = it->second;
		callback_session->write(data);
	}
	return true;
}





int INetworkASIO::Update()
{
	ip::tcp::socket socket(io_service_);
	// 等待直到客户端连接进来
	acceptor_->accept(socket);
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

void ASIOCallback::OnAccept(Port listen_port, NetID netid, IP ip, Port port)
{
	std::cout<<"listen_port:"<<listen_port<<" netid:"<<netid<<" ip"<<ip<<" port"<<port<<std::endl;
}

void ASIOCallback::OnRecv(NetID netid, const char *data, int length)
{
	std::cout<<"netid:"<<netid<<" data:"<<data<<" length:"<<length<<std::endl;
}

void ASIOCallback::OnDisconnect(NetID netid)
{
	std::cout<<"OnDisconnect netid:"<<netid<<std::endl;
}

ASIOCallback::ASIOCallback()
{

}

ASIOCallback::~ASIOCallback()
{

}
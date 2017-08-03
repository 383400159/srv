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
	//һ��session����һ�����
	session_ptr new_session( new INetworkSession(io_service_pool_.get_io_service(),io_service_work_pool_.get_io_service()));
	acceptor_->async_accept(new_session->socket(), boost::bind(&INetworkASIO::handle_accept,this,new_session,boost::asio::placeholders::error));
	return 1;
}
/*
	�����������
	@port			Ҫ�����Ķ˿ں�
	@backlog		������İ뿪���ӵ�������
	@netid_out		out�����������ɹ���ò���Ϊ���������netid��ע�⣬���ټ������ʱʹ��Disconnect������������netid��Ϊ�������
	@ip_bind		listen ʱ�İ�socket �� ip�����Ϊ0��󶨵�INADDR_ANY
	@Return		�Ƿ�����ɹ�
*/
bool INetworkASIO::Listen(Port port, int backlog, NetID netid_out, const char *ip_bind)
{
	// ����asio�඼��Ҫio_service����
	acceptor_ = new ip::tcp::acceptor(io_service_, ip::tcp::endpoint(ip::tcp::v4(), port));
	this->Start();
	//��Ϊio_service.run������߳� ������һ���߳���������
	io_thread_.reset(new boost::thread(boost::bind(&INetworkASIO::Run, this))); 
	obj_io_thread_.reset(new boost::thread(boost::bind(&io_service_pool::run, io_service_pool_)));
	obj_work_thread_.reset(new boost::thread(boost::bind(&io_service_pool::run, io_service_work_pool_)));
	return true;
}

 void INetworkASIO::handle_accept(session_ptr callback_session, const boost::system::error_code& error)
 {
	 if (!error) 
	 { 
		 //����ҷ���һ��netid ����Ψһid
		 NetIDCount_++;
		 callback_session->set_netid(NetIDCount_);
		 callback_session->RegisterCallback(callback_);
		 callback_session->start();
		 NetIDList_.insert(std::make_pair(NetIDCount_,callback_session));
		 callback_session->write("hello my is server");
		 std::cout<<"Client:";
		 std::cout<<callback_session->socket().remote_endpoint().address()<<std::endl;
		 // ������Ϻ��������������io_service����Ϊû���¼��������������
	 }
	 else
	 {
		 std::cout <<"handle_accept error:" <<error.value();
		 //throw boost::system::system_error(error);  
	 }
	 //������ζ�Ҫ��������
	 Start();
 }
	/*
	 ������������
	 @ip			Ҫ���ӵ�Զ�˵�ַ
	 @port			Ҫ���ӵ�Զ�˶˿ں�
	 @netid_out		out���������ӳɹ�ʱnetid_outΪ�������ӵ�����id��
	 @time_out		�����������Ӷ����ĳ�ʱʱ�䣬��λ���룬Ĭ��Ϊ3000
	 @Return		�Ƿ����ӳɹ�
	*/
bool INetworkASIO::Connect(const char *ip, Port port, NetID netid_out, unsigned long time_out)
{
	session_ptr new_session( new INetworkSession(io_service_,io_service_));
	// ���Ӷ˵㣬����ʹ���˱������ӣ������޸�IP��ַ����Զ������
	ip::tcp::endpoint ep(ip::address_v4::from_string(ip), port);
	// ���ӷ�����
	boost::system::error_code ec;
	new_session->socket().async_connect(ep, boost::bind(&INetworkASIO::handle_connect,this,new_session,netid_out,boost::asio::placeholders::error));
	//��Ϊio_service.run������߳� ������һ���߳���������
	io_thread_.reset(new boost::thread(boost::bind(&INetworkASIO::Run, this))); 
	return true;
}

  /*
	 �첽ping�ص�
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
	������Ϣ����ע�⣬�ú����������������Ѿ��ɹ����ͣ����ǽ���Ϣ�ŵ������Ͷ������첽����
	���ĳ�����ӶϿ��ص�disconnectʱ���п��ܵ�����Ϣ����ʧ
	�ú�������Ϣ������˳�����б�֤��
	@neiid			����Ŀ�������id
	@data			���͵���Ϣ��
	@length		��Ϣ������
	@Return		�Ƿ�ɹ��ŵ������Ͷ�����
*/
bool INetworkASIO::Send(NetID netid, const char *data, unsigned int length)
{
	auto it = NetIDList_.find(netid);
	if (it!=NetIDList_.end())
	{
		//Ŀ�������ڿ��Է�����Ϣ
		session_ptr callback_session = it->second;
		callback_session->write(data);
	}
	return true;
}





int INetworkASIO::Update()
{
	ip::tcp::socket socket(io_service_);
	// �ȴ�ֱ���ͻ������ӽ���
	acceptor_->accept(socket);
	// ��ʾ���ӽ����Ŀͻ���
	std::cout << socket.remote_endpoint().address()<<" "<<socket.remote_endpoint().port()<< std::endl;
	// ��ͻ��˷���hello world!
	boost::system::error_code ec;
	socket.write_some(buffer("hello world!"), ec);

	// ���������ӡ������Ϣ
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
#include "INetworkModule.h"




INetworkModule::INetworkModule(void)
{
	NetIDCount_ = 0;
}


INetworkModule::~INetworkModule(void)
{
	this->Stop();
}



int INetworkModule::Init()
{

	return 0;
}

void INetworkModule::Run()
{
	io_service_.run();
}
int INetworkModule::Stop()
{
	io_service_.stop();
	io_thread_->join();

	return 1;
}
int INetworkModule::Start()
{
	//�½�һ��socket �����ȴ���ҽ���
	socket_ptr sock(new ip::tcp::socket(io_service_));
	//һ��session����һ�����
	session_ptr new_session( new INetworkSession(io_service_));
	session_ptr new_session1( new INetworkSession(io_service_));
	acceptor_->async_accept(new_session->socket(), boost::bind(&INetworkModule::handle_accept,this,new_session,boost::asio::placeholders::error));
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
bool INetworkModule::Listen(Port port, int backlog, NetID *netid_out, const char *ip_bind)
{
	// ����asio�඼��Ҫio_service����
	acceptor_ = new ip::tcp::acceptor(io_service_, ip::tcp::endpoint(ip::tcp::v4(), port));
	this->Start();
	//��Ϊio_service.run������߳� ������һ���߳���������
	io_thread_.reset(new boost::thread(boost::bind(&INetworkModule::Run, this))); 

	return true;
}

 void INetworkModule::handle_accept(session_ptr callback_session, const boost::system::error_code& error)
 {
	 if (!error) 
	 { 
		 //����ҷ���һ��netid ����Ψһid
		 NetIDCount_++;
		 callback_session->set_netid(NetIDCount_);
		 NetIDList_.insert(std::make_pair(NetIDCount_,callback_session));
		 callback_session->start();
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
bool INetworkModule::Connect(const char *ip, Port port, NetID *netid_out, unsigned long time_out)
{
	// socket����
	ip::tcp::socket socket(io_service_);
	// ���Ӷ˵㣬����ʹ���˱������ӣ������޸�IP��ַ����Զ������
	ip::tcp::endpoint ep(ip::address_v4::from_string(ip), port);
	// ���ӷ�����
	boost::system::error_code ec;
	socket.connect(ep,ec);
	// ���������ӡ������Ϣ
	if(ec)
	{
		std::cout << boost::system::system_error(ec).what() << std::endl;
		return false;
	}
	// ��������
	char buf[100];
	size_t len=socket.read_some(buffer(buf), ec);
	std::cout.write(buf, len);
	return true;
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
bool INetworkModule::Send(NetID netid, const char *data, unsigned int length)
{
	
	return true;
}





int INetworkModule::Update()
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
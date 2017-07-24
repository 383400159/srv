#include "INetworkModule.h"


extern "C" {

	int write(int,void*,int);

	int read(int,void*,int);

};

INetworkModule::INetworkModule(void)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);//��ֹsocket�����׽���ʧ��
}


INetworkModule::~INetworkModule(void)
{
	//���WSADATA
	WSACleanup();
}



int INetworkModule::Init()
{
	return 0;
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


	SocketID = socket(PF_INET,SOCK_STREAM,0); //ָ��socket������

	init_server.sin_family = AF_INET;
	init_server.sin_addr.s_addr = INADDR_ANY;  //����˽��������ַ
	init_server.sin_port = htons(port);

	bind(SocketID,(struct sockaddr * )&init_server,sizeof(init_server)); //���ü����˿ں�
	listen(SocketID,backlog);	//������ʼ ������


	



	
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
	auto ret = send(netid,data,sizeof(data),0); //������д��size���ֽڵ�����
	if (ret!=-1)
	{
		std::cout<<"send ....."<<std::endl;
		return true;
	}
	else
	{
		std::cout<<"send bail....."<<std::endl;
		return false;
	}
}





int INetworkModule::Update()
{
	fd_set fdread;
	timeval fd_tv;

	FD_ZERO(&fdread);//��ʼ��fd_set
	FD_SET(SocketID,&fdread);///�����׽��־������Ӧ��fd_set

	fd_tv.tv_sec=0.0001;//�������Ǵ�����select�ȴ�����󷵻أ����ⱻ������Ҳ�������Ϸ���
	fd_tv.tv_usec=0;
	SOCKET  AcceptSocket;
	auto s_ret =  select(0,&fdread,NULL,NULL,&fd_tv);
	if (FD_ISSET(SocketID,&fdread)) ///����׽��־������fd_set�˵���ͻ����Ѿ���connect�����󷢹����ˣ����Ͽ���accept�ɹ�
	{
		struct sockaddr_in  accept_server ;

		int len=sizeof(SOCKADDR);
		AcceptSocket = accept(SocketID,(struct sockaddr * )&accept_server,&len); 
		auto iter = AcceptMapList.find(AcceptSocket);
		if(iter==AcceptMapList.end()){
			IEngineNetCallback* callback  = new IEngineNetCallback();
			AcceptMapList.insert(std::make_pair(AcceptSocket,callback));
			//this->Send(AcceptSocket,"test",10);
			callback->OnAccept(init_server.sin_port,AcceptSocket,accept_server.sin_addr.s_addr,accept_server.sin_port);
			
			
		}
		
	}

	fd_set clientread;
	FD_ZERO(&clientread);//��ʼ��fd_set

	//�����Ƿ�����Ϣ����	
	for (auto it = AcceptMapList.begin();it!=AcceptMapList.end();it++)
	{
			FD_SET(it->first,&clientread);///�����׽��־������Ӧ��fd_set
	}
	auto clent_ret =  select(0,&clientread,NULL,NULL,&fd_tv);

	for (auto it = AcceptMapList.begin();it!=AcceptMapList.end();it++)
	{
		if (FD_ISSET(it->first,&clientread))
		{
				
			char buf[100];
			size_t size = recv(it->first,buf,100,0);				//�ڶ�������100���ֽ�֮ǰһֱ�ȴ�
			auto callback = it->second;
			callback->OnRecv(it->first,buf,sizeof(buf));
		}
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
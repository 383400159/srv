#include "INetworkModule.h"


extern "C" {

	int write(int,void*,int);

	int read(int,void*,int);

};

INetworkModule::INetworkModule(void)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);//防止socket生成套接字失败
}


INetworkModule::~INetworkModule(void)
{
	//清除WSADATA
	WSACleanup();
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


	SocketID = socket(PF_INET,SOCK_STREAM,0); //指定socket的类型

	init_server.sin_family = AF_INET;
	init_server.sin_addr.s_addr = INADDR_ANY;  //服务端接受任意地址
	init_server.sin_port = htons(port);

	bind(SocketID,(struct sockaddr * )&init_server,sizeof(init_server)); //设置监听端口号
	listen(SocketID,backlog);	//监听开始 待机中


	



	
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
	auto ret = send(netid,data,sizeof(data),0); //正常则写入size个字节的数据
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

	FD_ZERO(&fdread);//初始化fd_set
	FD_SET(SocketID,&fdread);///分配套接字句柄到相应的fd_set
	fd_tv.tv_sec=0.1;//这里我们打算让select等待两秒后返回，避免被锁死，也避免马上返回
	fd_tv.tv_usec=0;
	SOCKET  AcceptSocket;
	select(0,&fdread,NULL,NULL,&fd_tv);
	if (FD_ISSET(SocketID,&fdread)) ///如果套接字句柄还在fd_set里，说明客户端已经有connect的请求发过来了，马上可以accept成功
	{
		struct sockaddr_in  accept_server ;

		int len=sizeof(SOCKADDR);
		AcceptSocket = accept(SocketID,(struct sockaddr * )&accept_server,&len); 
		auto iter = AcceptMapList.find(AcceptSocket);
		if(iter==AcceptMapList.end()){
			IEngineNetCallback* callback  = new IEngineNetCallback();
			AcceptMapList.insert(std::make_pair(AcceptSocket,callback));
			this->Send(AcceptSocket,"test",10);
			callback->OnAccept(init_server.sin_port,AcceptSocket,accept_server.sin_addr.s_addr,accept_server.sin_port);
		}
		
	}

	return 0;
}

void IEngineNetCallback::OnAccept(Port listen_port, NetID netid, IP ip, Port port)
{
	std::cout<<" listen_port:"<<listen_port<<" netid:"<<netid<<" ip"<<ip<<" port"<<port<<std::endl;
}

IEngineNetCallback::IEngineNetCallback()
{

}

IEngineNetCallback::~IEngineNetCallback()
{

}
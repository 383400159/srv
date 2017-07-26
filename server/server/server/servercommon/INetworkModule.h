#pragma once

#ifndef INETWORKMODULE_H
#define INETWORKMODULE_H
#ifdef _MSC_VER
#define _WIN32_WINNT 0x0501
#endif
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/thread.hpp> 
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include <vector>
#include <map>

using namespace boost::asio;


static const char NETWORK_MODULE[] = "NetworkModule";
class INetworkSession;
typedef	unsigned int		NetID;
typedef unsigned int		IP;
typedef unsigned short		Port;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
typedef boost::shared_ptr<INetworkSession> session_ptr;

class IEngineNetCallback
{
public:
	 IEngineNetCallback();
	 ~IEngineNetCallback();

	/*
	 ��������acceptʱ��ص��ú���
	 @netid			�����ӵ�netid
	 @ip			Զ�˵�ַ��������
	 @port			Զ�˶˿ں�
	*/
	void OnAccept(Port listen_port, NetID netid, IP ip, Port port);

	/*
	 ���յ�������Ϣʱ�ص��ú���
	 @netid			������Ϣ��Դ��netid
	 @data			��������
	 @length		���ݳ���
	*/
	void OnRecv(NetID netid, const char *data, int length);

	/*
	 ��������Ͽ���ʱ��ص��ú���
	 @netid			�Ͽ����������ӵ�netid
	*/
	void OnDisconnect(NetID netid);

	/*
	 �������첽ConnectAsyn������غ�ص�
	 @result		���ӽ���Ƿ�ɹ�
	 @handle		ConnectAsynʱ���������handle
	 @netid			���ӳɹ�ʱ��netidʱ�������ӵ������id
	 @ip			����ip
	 @port			����port
	*/
	void OnConnect(bool result, int handle, NetID netid, IP ip, Port port);
};

//������� һ��session����һ�����
class INetworkSession: public boost::enable_shared_from_this<INetworkSession> 
{
public: 
	INetworkSession(boost::asio::io_service& io_service) 
		: socket_(io_service) 
	{ 
	
	} 

	//���ص�ǰ�Ĳ�������
	inline ip::tcp::socket& socket() { return socket_; } 

	//��ʼ������Ϣ����
	void start() ;
	//��ȡ��Ϣ�ص�
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred) ;
	//д����Ϣ�ص�
	void handle_write(const boost::system::error_code& error) ;

	void on_receive(boost::shared_ptr<std::vector<char> > buffers, size_t bytes_transferred) ;


private: 

	// The socket used to communicate with the client. 
	ip::tcp::socket socket_; 

};

class INetworkModule 
{
public:
	INetworkModule();
	~INetworkModule();
public:
	 int Init();
	 int Start();
	 int Update();
	 int Stop();
	 int Release();
	 void Free();
	 void Run();

	/*
	 ע��������Ϣ�ص�
	 @callback		����ص�
	*/
	 void RegisterCallback(IEngineNetCallback * callback);

	/*
	 �����������
	 @port			Ҫ�����Ķ˿ں�
	 @backlog		������İ뿪���ӵ�������
	 @netid_out		out�����������ɹ���ò���Ϊ���������netid��ע�⣬���ټ������ʱʹ��Disconnect������������netid��Ϊ�������
	 @ip_bind		listen ʱ�İ�socket �� ip�����Ϊ0��󶨵�INADDR_ANY
	 @Return		�Ƿ�����ɹ�
	*/
	 bool Listen(Port port, int backlog, NetID *netid_out=0, const char *ip_bind=0);

	/*
	 ������������
	 @ip			Ҫ���ӵ�Զ�˵�ַ
	 @port			Ҫ���ӵ�Զ�˶˿ں�
	 @netid_out		out���������ӳɹ�ʱnetid_outΪ�������ӵ�����id��
	 @time_out		�����������Ӷ����ĳ�ʱʱ�䣬��λ���룬Ĭ��Ϊ3000
	 @Return		�Ƿ����ӳɹ�
	*/
	 bool Connect(const char *ip, Port port, NetID *netid_out, unsigned long time_out=3000);

	/*
	 ������������
	 @ip			Ҫ���ӵ�Զ�˵�ַ��������
	 @port			Ҫ���ӵ�Զ�˶˿ں�
	 @netid_out		out���������ӳɹ�ʱnetid_outΪ�������ӵ�����id��
	 @time_out		�����������Ӷ����ĳ�ʱʱ�䣬��λ���룬Ĭ��Ϊ3000
	 @Return		�Ƿ����ӳɹ�
	*/
	 bool Connect(IP ip, Port port, NetID *netid_out, unsigned long time_out=3000);

	/*
	 �첽�����������ӣ����ӽ���ڻص�OnConnect�еõ�
	 @ip			Ҫ���ӵ�Զ�˵�ַ
	 @port			Ҫ���ӵ�Զ�˶˿ں�
	 @handle		out��������ΪOnConnect�ص�ʱ��һ����ʶ
	 @time_out		�����������Ӷ����ĳ�ʱʱ�䣬��λ���룬Ĭ��Ϊ3000
	 @Return		�ύ�첽���������Ƿ�ɹ�
	*/
	 bool ConnectAsyn(const char *ip, Port port, int *handle, unsigned long time_out=3000);

	/*
	 �첽�����������ӣ����ӽ���ڻص�OnConnect�еõ�
	 @ip			Ҫ���ӵ�Զ�˵�ַ��������
	 @port			Ҫ���ӵ�Զ�˶˿ں�
	 @handle		out��������ΪOnConnect�ص�ʱ��һ����ʶ
	 @time_out		�����������Ӷ����ĳ�ʱʱ�䣬��λ���룬Ĭ��Ϊ3000
	 @Return		�ύ�첽���������Ƿ�ɹ�
	*/
	 bool ConnectAsyn(IP ip, Port port, int *handle, unsigned long time_out=3000);

	/*
	 ������Ϣ����ע�⣬�ú����������������Ѿ��ɹ����ͣ����ǽ���Ϣ�ŵ������Ͷ������첽����
	 ���ĳ�����ӶϿ��ص�disconnectʱ���п��ܵ�����Ϣ����ʧ
	 �ú�������Ϣ������˳�����б�֤��
	 @neiid			����Ŀ�������id
	 @data			���͵���Ϣ��
	 @length		��Ϣ������
	 @Return		�Ƿ�ɹ��ŵ������Ͷ�����
	*/
	 bool Send(NetID netid, const char *data, unsigned int length);

	/*
	 �Ͽ�ĳ���ӣ����ԶϿ����������׽������ڵ���������
	 ע�⣬�ú�������ʱ��������������Ѿ��Ͽ������ǽ������ӱ��Ϊ���Ͽ�
	 �����Ͽ�����Ϊ����IEngineNetCallback::OnDisconnect��֤��
	 @netid			Ҫ�Ͽ�������id
	*/
	 void Disconnect(NetID netid);

	 /*
	 �첽���ӻص�
	 */
	 void handle_accept(socket_ptr callback_session, const boost::system::error_code& error); 

private:
	boost::asio::io_service io_service_;
	boost::asio::ip::tcp::acceptor* acceptor_;
	boost::shared_ptr<boost::thread> io_thread_; 

	std::map<ip::tcp::socket,IEngineNetCallback*> AcceptMapList;
	IEngineNetCallback* callback_;
};

#endif

#pragma once

#ifndef INETWORKMODULE_H
#define INETWORKMODULE_H
#ifdef _MSC_VER
#define _WIN32_WINNT 0x0501
#endif
#include <boost/thread.hpp> 
#include <boost/aligned_storage.hpp> 
#include <boost/array.hpp> 
#include <boost/bind.hpp> 
#include <boost/enable_shared_from_this.hpp> 
#include <boost/noncopyable.hpp> 
#include <boost/shared_ptr.hpp> 
#include <boost/asio.hpp> 
#include <boost/date_time/posix_time/posix_time_types.hpp>

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
typedef boost::shared_ptr<std::string> string_ptr;

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
		, io_work_service(io_service)
		, timer_(io_service)
	{ 
		is_read_ = false;
	} 

	//���ص�ǰ�Ĳ�������
	inline ip::tcp::socket& socket() { return socket_; } 
	inline void set_netid(uint32_t netid) { netid_ = netid;}
	inline uint32_t get_netid() {return netid_ ;}


	//��ʼ������Ϣ����
	void start() {
		//���ͻ���������5�����Ժ����Ƿ������ͨ��
		timer_.expires_from_now(boost::posix_time::seconds(5));
		timer_.async_wait(boost::bind(&INetworkSession::time_out, shared_from_this()));
		this->start_read();
	}

	void start_read(){
		socket_.async_read_some(boost::asio::buffer(data_), 
			boost::bind(&INetworkSession::handle_read, 
			shared_from_this(), 
			boost::asio::placeholders::error, 
			boost::asio::placeholders::bytes_transferred)); 
	}
	//��ȡ��Ϣ�ص�
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
		if (!error) 
		{ 
			is_read_ = true;
			//ȡ����ʱ��
			timer_.cancel();
			boost::shared_ptr<std::vector<char> > buf(new std::vector<char>); 

			buf->resize(bytes_transferred); 
			std::copy(data_.begin(), data_.begin() + bytes_transferred, buf->begin()); 
			//��ֹ�������ݶ��� ����post���д�����Ϣ����
			io_work_service.post(boost::bind(&INetworkSession::on_receive, 
				shared_from_this(), 
				buf, 
				bytes_transferred));
			this->start_read();
		} 
	}
	//д����Ϣ�ص�
	void handle_write(const boost::system::error_code& error) {
		if (!error) 
		{ 
			send_data_list_.erase(send_data_list_.begin());
			//std::cout<<" handle_write:"<<std::endl;
			if (send_data_list_.size()>0)
			{
				this->async_write(send_data_list_[0]);
			}
		} 
	}

	void on_receive(boost::shared_ptr<std::vector<char> > buffers, size_t bytes_transferred) 
	{
		char* data_stream = &(*buffers->begin()); 
		// in here finish the work. 
		std::cout<<" port:"<<socket_.remote_endpoint().port() << "receive :" << bytes_transferred << " bytes." << 
			"message :" << data_stream << std::endl; 
		//������Ϣ�б�
		send_data_list_.push_back((std::string)data_stream);
		//test ������Ϣ��clent
		if (send_data_list_.size()==1)
		{
			this->async_write((std::string)data_stream);
		}
		
	}

	//ͬ��д����Ϣ
	void write(std::string  data){
		socket_.write_some(buffer(data));
	}
	//�첽д����Ϣ
	void async_write(std::string  data){
		str_prt_.reset(new std::string(data));
		boost::asio::async_write(socket_,buffer(*str_prt_),boost::bind(&INetworkSession::handle_write, 
			shared_from_this(), 
			boost::asio::placeholders::error)); 
	}

private: 
	void time_out()
	{
		//�ж�û�дӿͻ��˶�ȡ����Ϣ������¶Ͽ�����
		if(!is_read_){
			socket_.close();
			
		}
	}

	boost::asio::io_service& io_work_service; 
	// The socket used to communicate with the client. 
	ip::tcp::socket socket_; 
	uint32_t netid_ ;
	boost::array<char, 1024> data_; 
	//��ֹд����Ϣ �ֲ�����������
	string_ptr str_prt_;
	//˳������Ϣ ��ֹ��Ϣ����
	std::vector<std::string> send_data_list_;

	deadline_timer timer_;
	bool is_read_ ;
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
	 void handle_accept(session_ptr callback_session, const boost::system::error_code& error); 

private:
	boost::asio::io_service io_service_;
	boost::asio::ip::tcp::acceptor* acceptor_;
	boost::shared_ptr<boost::thread> io_thread_; 

	std::map<ip::tcp::socket,IEngineNetCallback*> AcceptMapList_;
	IEngineNetCallback* callback_;

	uint32_t NetIDCount_;
	std::map<uint32_t,session_ptr> NetIDList_;
};

#endif

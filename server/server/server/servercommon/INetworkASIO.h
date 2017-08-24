#ifndef INETWORKASIO_H
#define INETWORKASIO_H
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


#include "IModule.h"
using namespace boost::asio;



class INetworkSession;
typedef	unsigned int		NetID;
typedef unsigned int		IP;
typedef unsigned short		Port;
typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;
typedef boost::shared_ptr<INetworkSession> session_ptr;
typedef boost::shared_ptr<std::string> string_ptr;

class ASIOCallback
{
public:
	 ASIOCallback();
	 ~ASIOCallback();

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
	INetworkSession(boost::asio::io_service& io_service,boost::asio::io_service& io_work_service) 
		: socket_(io_service) 
		, io_work_service_(io_service)
		, timer_(io_service)
	{ 
		is_read_ = false;
	} 

	//���ص�ǰ�Ĳ�������
	inline ip::tcp::socket& socket() { return socket_; } 
	inline void set_netid(uint32_t netid) { netid_ = netid;}
	inline uint32_t get_netid() {return netid_ ;}

	void RegisterCallback(ASIOCallback * callback){
		callback_ = callback;
	}

	//��ʼ������Ϣ����
	void start() {
		//���ͻ���������5�����Ժ����Ƿ������ͨ��
		timer_.expires_from_now(boost::posix_time::seconds(5));
		timer_.async_wait(boost::bind(&INetworkSession::time_out, shared_from_this()));
		this->start_read();
		
		callback_->OnAccept(socket_.local_endpoint().port(),netid_,inet_addr(socket_.remote_endpoint().address().to_string().c_str()),socket_.remote_endpoint().port());
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
			io_work_service_.post(boost::bind(&INetworkSession::on_receive, 
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



	//д����Ϣ
	void write(std::string  data){
		//������Ϣ�б�
		send_data_list_.push_back(data);
		//test ������Ϣ��clent
		if (send_data_list_.size()==1)
		{
			this->async_write((std::string)data);
		}
	}


private: 

	/*
	* 1�� str += "a"�� str =str+ "a" Ч�ʲ�ܶ౶����   ǰ�����ܸߺܶ� 
	* 2�������ͳ������Ч�ʺܵͣ�����str = str2+"a"����   ����ȷʵ��һЩ�������ӵ�����Ƽ���append   str.append(str2).append("a").append(str3);
	* 
	* @result 
	* Ч���ڳ����б�ע�ˣ��������£� 
	* 1��a = a + b �� a += b����£��ú��߿��Դ�����Ч�ʣ� 
	* 2���ж������������ƴ���ٸ�ֵ������£��Ƽ�append������+=������Ҫa = a + b +c 
	* 3��û�б�Ҫ��Ҫ�ظ�����ֲ��������Ứ�϶��ʱ�� 
	* 
	*/ 
	void on_receive(boost::shared_ptr<std::vector<char> > buffers, size_t bytes_transferred) 
	{
		byte* data_stream = (byte*)(&(*buffers->begin())); 
		//��Ҫ�����Լ���ռ��4��
		int max_len = data_stream[0]+data_stream[1]*256+data_stream[2]*256*256+data_stream[3]*256*256*256 + 4;
		short msgID = data_stream[4]+data_stream[5]*256;
		
		if (max_len!=buffers->size())
		{
			std::cout<<"on_receive len error "<<max_len <<" "<<buffers->size()<<std::endl;
			return;
		}

		// in here finish the work. 
		
		std::cout<<" port:"<<socket_.remote_endpoint().port() << "receive :" << bytes_transferred << " bytes." << 
			"message :" << data_stream+12 << std::endl; 
			

		std::string msg ;
		for (auto i = 12; i<max_len;i++)
		{
			msg += data_stream[i]; 
		}
		callback_->OnRecv(netid_,msg.c_str(),max_len-12);
	}

	//ƴ����Ϣ
	void create_proto_msg(short msgID,std::string data,uint32_t & send_len){
		byte m_NotUseByte[] = {0,0};
		byte m_NotUseByte2[] = {0,0,0,0};
		//short to byte
		byte net_MsgID_byte[2]; 
		net_MsgID_byte[0] = (byte)(msgID);
		net_MsgID_byte[1] = (byte)(msgID >> 8);

		int net_data_size = sizeof(net_MsgID_byte) +  sizeof(m_NotUseByte) + sizeof(m_NotUseByte2) + data.size();
		//int to byte
		byte net_Data_Size_byte[4];
		net_Data_Size_byte[0] = (byte)(net_data_size);
		net_Data_Size_byte[1] = (byte)(net_data_size >> 8);
		net_Data_Size_byte[2] = (byte)(net_data_size >> 16);
		net_Data_Size_byte[3] = (byte)(net_data_size >> 24);

		int i = 0;
		std::copy(&net_Data_Size_byte[0], &net_Data_Size_byte[sizeof(net_Data_Size_byte)] , send_msg_.begin()+i); 
		i = i + sizeof(net_Data_Size_byte);
		std::copy(&net_MsgID_byte[0], &net_MsgID_byte[sizeof(net_MsgID_byte)] , send_msg_.begin()+i); 
		i = i + sizeof(net_MsgID_byte);
		std::copy(&m_NotUseByte[0], &m_NotUseByte[sizeof(m_NotUseByte)] , send_msg_.begin()+i); 
		i = i + sizeof(m_NotUseByte);
		std::copy(&m_NotUseByte2[0], &m_NotUseByte2[sizeof(m_NotUseByte2)] , send_msg_.begin()+i); 
		i = i + sizeof(m_NotUseByte2);
		std::copy(data.begin(), data.end() , send_msg_.begin()+i); 
		i = i + data.size();
		send_len = i;
	}



	//�첽д����Ϣ
	void async_write(std::string  data){
		void *send_data = nullptr; uint32_t send_len = 0;
		create_proto_msg((short)2001,data,send_len);
		//std::copy(data.begin(), data.end() , send_msg_.begin()); 
		boost::asio::async_write(socket_,buffer(send_msg_,send_len),boost::bind(&INetworkSession::handle_write, 
			shared_from_this(), 
			boost::asio::placeholders::error)); 
	}

	void time_out()
	{
		//�ж�û�дӿͻ��˶�ȡ����Ϣ������¶Ͽ�����
		if(!is_read_&&false){
			socket_.close();
			//netid �޳�
			callback_->OnDisconnect(netid_);
		}
	}

	typedef unsigned char byte;
	byte m_Buffer[1024 * 1024 * 4];

	boost::asio::io_service& io_work_service_; 
	// The socket used to communicate with the client. 
	ip::tcp::socket socket_; 
	uint32_t netid_ ;
	boost::array<char, 1024> data_;
	//��ֹд����Ϣ �ֲ�����������
	boost::array<byte, 1024> send_msg_; 
	//˳������Ϣ ��ֹ��Ϣ����
	std::vector<std::string> send_data_list_;
	//�ص�
	ASIOCallback* callback_;
	
	deadline_timer timer_;
	bool is_read_ ;
};



/// A pool of io_service objects. 
class io_service_pool 
{ 
public: 
	/// Construct the io_service pool. 
	explicit io_service_pool(std::size_t pool_size) : next_io_service_(0) 
	{ 
		if (pool_size == 0) 
			throw std::runtime_error("io_service_pool size is 0"); 

		// Give all the io_services work to do so that their run() functions will not 
		// exit until they are explicitly stopped. 
		for (std::size_t i = 0; i < pool_size; ++i) 
		{ 
			io_service_ptr io_service(new boost::asio::io_service); 
			work_ptr work(new boost::asio::io_service::work(*io_service)); 
			io_services_.push_back(io_service); 
			work_.push_back(work); 
		} 
	} 

	// Run all io_service objects in the pool. 
	void run() 
	{ 
		// Create a pool of threads to run all of the io_services. 
		std::vector<boost::shared_ptr<boost::thread> > threads; 
		for (std::size_t i = 0; i < io_services_.size(); ++i) 
		{ 
			boost::shared_ptr<boost::thread> thread(new boost::thread( 
				boost::bind(&boost::asio::io_service::run, io_services_[i]))); 
			threads.push_back(thread); 
		} 

		// Wait for all threads in the pool to exit. 
		for (std::size_t i = 0; i < threads.size(); ++i) 
			threads[i]->join(); 
	} 

	// Stop all io_service objects in the pool. 
	void stop() 
	{ 
		// Explicitly stop all io_services. 
		for (std::size_t i = 0; i < io_services_.size(); ++i) 
			io_services_[i]->stop(); 
	} 

	// Get an io_service to use. 
	boost::asio::io_service& get_io_service() 
	{ 
		// Use a round-robin scheme to choose the next io_service to use. 
		boost::asio::io_service& io_service = *io_services_[next_io_service_]; 
		++next_io_service_; 
		if (next_io_service_ == io_services_.size()) 
			next_io_service_ = 0; 
		//std::cout<<" next_io_service_:"<<next_io_service_<<std::endl;
		return io_service; 
	} 

private: 
	typedef boost::shared_ptr<boost::asio::io_service> io_service_ptr; 
	typedef boost::shared_ptr<boost::asio::io_service::work> work_ptr; 

	/// The pool of io_services. 
	std::vector<io_service_ptr> io_services_; 

	/// The work that keeps the io_services running. 
	std::vector<work_ptr> work_; 

	/// The next io_service to use for a connection. 
	std::size_t next_io_service_; 
}; 




class INetworkASIO : public IModule
{
public:
	INetworkASIO(std::size_t io_service_pool_size);
	INetworkASIO();
	~INetworkASIO();
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
	 //void RegisterCallback(ASIOCallback * callback);

	/*
	 �����������
	 @port			Ҫ�����Ķ˿ں�
	 @backlog		������İ뿪���ӵ�������
	 @netid_out		out�����������ɹ���ò���Ϊ���������netid��ע�⣬���ټ������ʱʹ��Disconnect������������netid��Ϊ�������
	 @ip_bind		listen ʱ�İ�socket �� ip�����Ϊ0��󶨵�INADDR_ANY
	 @Return		�Ƿ�����ɹ�
	*/
	 bool Listen(Port port, int backlog, NetID netid_out=0, const char *ip_bind=0);

	/*
	 ������������
	 @ip			Ҫ���ӵ�Զ�˵�ַ
	 @port			Ҫ���ӵ�Զ�˶˿ں�
	 @netid_out		out���������ӳɹ�ʱnetid_outΪ�������ӵ�����id��
	 @time_out		�����������Ӷ����ĳ�ʱʱ�䣬��λ���룬Ĭ��Ϊ3000
	 @Return		�Ƿ����ӳɹ�
	*/
	 bool Connect(const char *ip, Port port, NetID netid_out, unsigned long time_out=3000);


	/*
	 ������������
	 @ip			Ҫ���ӵ�Զ�˵�ַ��������
	 @port			Ҫ���ӵ�Զ�˶˿ں�
	 @netid_out		out���������ӳɹ�ʱnetid_outΪ�������ӵ�����id��
	 @time_out		�����������Ӷ����ĳ�ʱʱ�䣬��λ���룬Ĭ��Ϊ3000
	 @Return		�Ƿ����ӳɹ�
	*/
	 //bool Connect(IP ip, Port port, NetID *netid_out, unsigned long time_out=3000);

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
	 �����Ͽ�����Ϊ����ASIOCallback::OnDisconnect��֤��
	 @netid			Ҫ�Ͽ�������id
	*/
	 void Disconnect(NetID netid);

	 /*
	 �첽���ӻص�
	 */
	 void handle_accept(session_ptr callback_session, const boost::system::error_code& error); 

	  /*
	 �첽ping�ص�
	 */
	 void handle_connect(session_ptr callback_session,NetID netid_out, const boost::system::error_code& error); 

private:
	boost::asio::io_service io_service_;
	boost::asio::ip::tcp::acceptor* acceptor_;

	boost::shared_ptr<boost::thread> io_thread_; 
	boost::shared_ptr<boost::thread> obj_io_thread_; 
	boost::shared_ptr<boost::thread> obj_work_thread_; 

	std::map<ip::tcp::socket,ASIOCallback*> AcceptMapList_;
	ASIOCallback* callback_;

	uint32_t NetIDCount_;
	std::map<uint32_t,session_ptr> NetIDList_;
	std::map<uint32_t,session_ptr> ServerList_;


	io_service_pool io_service_pool_; 
	io_service_pool io_service_work_pool_; 
};

#endif

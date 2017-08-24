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
	 单有连接accept时则回调该函数
	 @netid			该连接的netid
	 @ip			远端地址，主机序
	 @port			远端端口号
	*/
	void OnAccept(Port listen_port, NetID netid, IP ip, Port port);

	/*
	 当收到网络消息时回调该函数
	 @netid			网络消息来源的netid
	 @data			网络数据
	 @length		数据长度
	*/
	void OnRecv(NetID netid, const char *data, int length);

	/*
	 当有网络断开的时候回调该函数
	 @netid			断开的网络连接的netid
	*/
	void OnDisconnect(NetID netid);

	/*
	 当调用异步ConnectAsyn结果返回后回调
	 @result		连接结果是否成功
	 @handle		ConnectAsyn时的输出参数handle
	 @netid			连接成功时，netid时所得连接的网络层id
	 @ip			连接ip
	 @port			连接port
	*/
	void OnConnect(bool result, int handle, NetID netid, IP ip, Port port);
};

//绑定玩家类 一个session就是一个玩家
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

	//返回当前的操作对象
	inline ip::tcp::socket& socket() { return socket_; } 
	inline void set_netid(uint32_t netid) { netid_ = netid;}
	inline uint32_t get_netid() {return netid_ ;}

	void RegisterCallback(ASIOCallback * callback){
		callback_ = callback;
	}

	//开始监听消息到来
	void start() {
		//当客户端连接上5秒钟以后检测是否进行了通信
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
	//读取消息回调
	void handle_read(const boost::system::error_code& error, size_t bytes_transferred) {
		if (!error) 
		{ 
			is_read_ = true;
			//取消定时器
			timer_.cancel();
			boost::shared_ptr<std::vector<char> > buf(new std::vector<char>); 

			buf->resize(bytes_transferred); 
			std::copy(data_.begin(), data_.begin() + bytes_transferred, buf->begin()); 
			//防止解析数据堵塞 采用post队列处理消息解析
			io_work_service_.post(boost::bind(&INetworkSession::on_receive, 
				shared_from_this(), 
				buf, 
				bytes_transferred));
			this->start_read();
		} 
	}
	//写入消息回调
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



	//写入消息
	void write(std::string  data){
		//加入消息列表
		send_data_list_.push_back(data);
		//test 发送消息给clent
		if (send_data_list_.size()==1)
		{
			this->async_write((std::string)data);
		}
	}


private: 

	/*
	* 1、 str += "a"， str =str+ "a" 效率差很多倍？？   前者性能高很多 
	* 2、变量和常量相加效率很低？？如str = str2+"a"？？   性能确实低一些，多个相加的情况推荐用append   str.append(str2).append("a").append(str3);
	* 
	* @result 
	* 效果在程序中标注了，结论如下： 
	* 1、a = a + b 与 a += b情况下，用后者可以大大提高效率； 
	* 2、有多个变量，常量拼接再赋值的情况下，推荐append，而非+=，更不要a = a + b +c 
	* 3、没有必要不要重复定义局部变量，会花较多的时间 
	* 
	*/ 
	void on_receive(boost::shared_ptr<std::vector<char> > buffers, size_t bytes_transferred) 
	{
		byte* data_stream = (byte*)(&(*buffers->begin())); 
		//需要加上自己所占的4格
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

	//拼接消息
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



	//异步写入消息
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
		//判断没有从客户端读取到消息的情况下断开连接
		if(!is_read_&&false){
			socket_.close();
			//netid 剔除
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
	//防止写入消息 局部变量被擦除
	boost::array<byte, 1024> send_msg_; 
	//顺序发送消息 防止消息错乱
	std::vector<std::string> send_data_list_;
	//回调
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
	 注册网络消息回调
	 @callback		网络回调
	*/
	 //void RegisterCallback(ASIOCallback * callback);

	/*
	 创建监听句柄
	 @port			要监听的端口号
	 @backlog		所允许的半开连接的最大个数
	 @netid_out		out参数，监听成功则该参数为监听句柄的netid。注意，销毁监听句柄时使用Disconnect函数，并将该netid作为传入参数
	 @ip_bind		listen 时的绑定socket 的 ip，如果为0则绑定到INADDR_ANY
	 @Return		是否监听成功
	*/
	 bool Listen(Port port, int backlog, NetID netid_out=0, const char *ip_bind=0);

	/*
	 建立网络连接
	 @ip			要连接的远端地址
	 @port			要连接的远端端口号
	 @netid_out		out参数，连接成功时netid_out为建立连接的网络id号
	 @time_out		建立网络连接动作的超时时间，单位毫秒，默认为3000
	 @Return		是否连接成功
	*/
	 bool Connect(const char *ip, Port port, NetID netid_out, unsigned long time_out=3000);


	/*
	 建立网络连接
	 @ip			要连接的远端地址，主机序
	 @port			要连接的远端端口号
	 @netid_out		out参数，连接成功时netid_out为建立连接的网络id号
	 @time_out		建立网络连接动作的超时时间，单位毫秒，默认为3000
	 @Return		是否连接成功
	*/
	 //bool Connect(IP ip, Port port, NetID *netid_out, unsigned long time_out=3000);

	/*
	 发送消息包，注意，该函数并不代表数据已经成功发送，而是将消息放到待发送队列中异步发送
	 如果某个连接断开回调disconnect时，有可能导致消息包丢失
	 该函数对消息包发送顺序是有保证的
	 @neiid			发送目标的网络id
	 @data			发送的消息包
	 @length		消息包长度
	 @Return		是否成功放到待发送队列中
	*/
	 bool Send(NetID netid, const char *data, unsigned int length);

	/*
	 断开某连接，可以断开包括监听套接字在内的所有连接
	 注意，该函数返回时并不代表该连接已经断开，而是将该连接标记为待断开
	 真正断开的行为是由ASIOCallback::OnDisconnect保证的
	 @netid			要断开的网络id
	*/
	 void Disconnect(NetID netid);

	 /*
	 异步链接回调
	 */
	 void handle_accept(session_ptr callback_session, const boost::system::error_code& error); 

	  /*
	 异步ping回调
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

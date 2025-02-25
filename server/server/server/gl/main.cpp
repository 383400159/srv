#ifdef _MSC_VER
#define _WIN32_WINNT 0x0501
#endif
#include <stdio.h> 
#include <cstdlib> 
#include <iostream> 
#include <boost/thread.hpp> 
#include <boost/aligned_storage.hpp> 
#include <boost/array.hpp> 
#include <boost/bind.hpp> 
#include <boost/enable_shared_from_this.hpp> 
#include <boost/noncopyable.hpp> 
#include <boost/shared_ptr.hpp> 
#include <boost/asio.hpp> 

using boost::asio::ip::tcp; 

class handler_allocator 
	: private boost::noncopyable 
{ 
public: 
	handler_allocator() 
		: in_use_(false) 
	{ 
	} 

	void* allocate(std::size_t size) 
	{ 
		if (!in_use_ && size < storage_.size) 
		{ 
			in_use_ = true; 
			return storage_.address(); 
		} 
		else 
		{ 
			return ::operator new(size); 
		} 
	} 

	void deallocate(void* pointer) 
	{ 
		if (pointer == storage_.address()) 
		{ 
			in_use_ = false; 
		} 
		else 
		{ 
			::operator delete(pointer); 
		} 
	} 

private: 
	// Storage space used for handler-based custom memory allocation. 
	boost::aligned_storage<1024> storage_; 

	// Whether the handler-based custom allocation storage has been used. 
	bool in_use_; 
}; 

template <typename Handler> 
class custom_alloc_handler 
{ 
public: 
	custom_alloc_handler(handler_allocator& a, Handler h) 
		: allocator_(a), 
		handler_(h) 
	{ 
	} 

	template <typename Arg1> 
	void operator()(Arg1 arg1) 
	{ 
		handler_(arg1); 
	} 

	template <typename Arg1, typename Arg2> 
	void operator()(Arg1 arg1, Arg2 arg2) 
	{ 
		handler_(arg1, arg2); 
	} 

	friend void* asio_handler_allocate(std::size_t size, 
		custom_alloc_handler<Handler>* this_handler) 
	{ 
		return this_handler->allocator_.allocate(size); 
	} 

	friend void asio_handler_deallocate(void* pointer, std::size_t /*size*/, 
		custom_alloc_handler<Handler>* this_handler) 
	{ 
		this_handler->allocator_.deallocate(pointer); 
	} 

private: 
	handler_allocator& allocator_; 
	Handler handler_; 
}; 

// Helper function to wrap a handler object to add custom allocation. 
template <typename Handler> 
inline custom_alloc_handler<Handler> make_custom_alloc_handler( 
	handler_allocator& a, Handler h) 
{ 
	return custom_alloc_handler<Handler>(a, h); 
} 

/// A pool of io_service objects. 
class io_service_pool 
	: private boost::noncopyable 
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
		std::cout<<" next_io_service_:"<<next_io_service_<<std::endl;
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

class session 
	: public boost::enable_shared_from_this<session> 
{ 
public: 
	session(boost::asio::io_service& work_service
		, boost::asio::io_service& io_service) 
		: socket_(io_service) 
		, io_work_service(work_service) 
	{ 
	} 

	tcp::socket& socket() 
	{ 
		return socket_; 
	} 

	void start() 
	{ 
		socket_.async_read_some(boost::asio::buffer(data_), 
			make_custom_alloc_handler(allocator_, 
			boost::bind(&session::handle_read, 
			shared_from_this(), 
			boost::asio::placeholders::error, 
			boost::asio::placeholders::bytes_transferred))); 
	} 

	void handle_read(const boost::system::error_code& error, 
		size_t bytes_transferred) 
	{ 
		if (!error) 
		{ 
			boost::shared_ptr<std::vector<char> > buf(new std::vector<char>); 

			buf->resize(bytes_transferred); 
			std::copy(data_.begin(), data_.begin() + bytes_transferred, buf->begin()); 
			io_work_service.post(boost::bind(&session::on_receive
				, shared_from_this(), buf, bytes_transferred)); 

			socket_.async_read_some(boost::asio::buffer(data_), 
				make_custom_alloc_handler(allocator_, 
				boost::bind(&session::handle_read, 
				shared_from_this(), 
				boost::asio::placeholders::error, 
				boost::asio::placeholders::bytes_transferred))); 
		} 
	} 

	void handle_write(const boost::system::error_code& error) 
	{ 
		if (!error) 
		{ 
		} 
	} 

	void on_receive(boost::shared_ptr<std::vector<char> > buffers
		, size_t bytes_transferred) 
	{ 
		char* data_stream = &(*buffers->begin()); 
		// in here finish the work. 
		std::cout<<" port:"<<socket_.remote_endpoint().port() << "receive :" << bytes_transferred << " bytes." << 
			"message :" << data_stream << std::endl; 
	} 

private: 
	// The io_service used to finish the work. 
	boost::asio::io_service& io_work_service; 

	// The socket used to communicate with the client. 
	tcp::socket socket_; 

	// Buffer used to store data received from the client. 
	boost::array<char, 1024> data_; 

	// The allocator to use for handler-based custom memory allocation. 
	handler_allocator allocator_; 
}; 

typedef boost::shared_ptr<session> session_ptr; 

class server 
{ 
public: 
	server(short port, std::size_t io_service_pool_size) 
		: io_service_pool_(io_service_pool_size) 
		, io_service_work_pool_(io_service_pool_size) 
		, m_work(m_io_service)
		, acceptor_(m_io_service, tcp::endpoint(tcp::v4(), port)) 
	{ 
		std::cout<<" start server..."<<std::endl;
		session_ptr new_session(new session(io_service_work_pool_.get_io_service()
			,io_service_pool_.get_io_service())); 
		acceptor_.async_accept(new_session->socket(), 
			boost::bind(&server::handle_accept, this, new_session, 
			boost::asio::placeholders::error)); 
	} 

	void handle_accept(session_ptr callback_session, 
		const boost::system::error_code& error) 
	{ 
		if (!error) 
		{ 
			callback_session->start(); 
			session_list.push_back(callback_session);
			std::cout<<" new client:"<< callback_session->socket().remote_endpoint().address()<<" "<<callback_session->socket().remote_endpoint().port()<< std::endl;
			session_ptr new_session(new session(io_service_work_pool_.get_io_service()
				, io_service_pool_.get_io_service())); 
			acceptor_.async_accept(new_session->socket(), 
				boost::bind(&server::handle_accept, this, new_session, 
				boost::asio::placeholders::error)); 
		} 
	} 

	void io_run()
	{
		m_io_service.run();
	}
	void run() 
	{ 
		io_thread_.reset(new boost::thread(boost::bind(&server::io_run
			, this))); 
		role_io_thread_.reset(new boost::thread(boost::bind(&io_service_pool::run
			, &io_service_pool_))); 
		role_work_thread_.reset(new boost::thread(boost::bind(&io_service_pool::run
			, &io_service_work_pool_))); 
	} 

	void stop() 
	{ 
		m_io_service.stop();
		io_service_pool_.stop(); 
		io_service_work_pool_.stop(); 

		io_thread_->join();
		role_io_thread_->join(); 
		role_work_thread_->join(); 
	} 

private: 
	boost::shared_ptr<boost::thread> io_thread_; 
	boost::shared_ptr<boost::thread> role_io_thread_; 
	boost::shared_ptr<boost::thread> role_work_thread_; 

	io_service_pool io_service_pool_; 
	io_service_pool io_service_work_pool_; 
	boost::asio::io_service m_io_service;
	boost::asio::io_service::work m_work;


	tcp::acceptor acceptor_; 
	std::vector<session_ptr> session_list;
}; 

int main(int argc, char* argv[]) 
{ 
	try 
	{ 

		using namespace std; // For atoi. 
		server s(7001,8); 

		s.run(); 

		getchar(); 

		s.stop(); 
	} 
	catch (std::exception& e) 
	{ 
		std::cerr << "Exception: " << e.what() << "/n"; 
	} 

	return 0; 
} 
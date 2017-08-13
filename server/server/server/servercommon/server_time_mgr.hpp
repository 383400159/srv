#ifndef __SERVER_TIME_MGR__
#define __SERVER_TIME_MGR__
#include <chrono>

#include <time.h>
using namespace std::chrono;

class ServerTimeMgr
{
	typedef std::chrono::duration<unsigned int, std::deca> second_type;
	typedef std::chrono::duration<unsigned long long, std::milli> milli_type;
	typedef std::chrono::duration<unsigned long long, std::nano> nano_type;
public:
	ServerTimeMgr(): /*server_time_(0),second_time_(0),*/millisecond_time_(0), nanosecond_time_(0) {}
	~ServerTimeMgr() {}

	static ServerTimeMgr& get_instance()
	{
		static ServerTimeMgr instance;
		return instance;
	}

	inline void Update()
	{

//		server_time_++;

		nanosecond_time_ = time_point_cast<nano_type>(system_clock::now()).time_since_epoch().count();

 		millisecond_time_ = nanosecond_time_ / 1000000;
// 		second_time_ = static_cast<unsigned int>(millisecond_time_ / 1000);
	}

// 	inline unsigned int get_server_time() { return server_time_; }
// 	inline unsigned int get_second_time() { return second_time_; }
 	inline unsigned long long get_millisecond_time() { return millisecond_time_; }
	inline unsigned long long get_nanosecond_time() { return nanosecond_time_; }
	inline unsigned long long GetMillisecondTime()
	{
		return time_point_cast<nano_type>(system_clock::now()).time_since_epoch().count() / 1000000; 
	}

private:
// 	unsigned int server_time_;
// 	unsigned int second_time_;
 	unsigned long long millisecond_time_;
	unsigned long long nanosecond_time_;
};

#endif
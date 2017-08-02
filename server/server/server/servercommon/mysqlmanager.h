#pragma once
#ifndef MYSQLMOANAGER_H
#define MYSQLMOANAGER_H

#ifdef _MSC_VER
#define _WIN32_WINNT 0x0501
#endif
#include <boost/thread.hpp> 
#include <boost/aligned_storage.hpp> 
#include <boost/bind.hpp> 
#include <boost/shared_ptr.hpp> 
#include <boost/date_time/posix_time/posix_time_types.hpp>

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include <vector>
#include <map>
#include "mysql.h"

#include "singleton.h"
#pragma comment(lib,"libmysql.lib") 
using namespace std;
typedef	unsigned int		NetID;
struct SQLID
{
	NetID netid;
	string sql;
};
typedef boost::shared_ptr<SQLID> SQLID_ptr;
class mysqlmanager : public Singleton<mysqlmanager> 
{
public:
	mysqlmanager(void);
	~mysqlmanager(void);
	//启动数据库
	bool start();



	//用户读取函数 
	void get_data_by_key(NetID netid,string key);
	void get_data_by_two_key(NetID netid,string key,string key1="");

	//用户写入函数
	void write_data_by_key(NetID netid,string key,string val);
	void write_data_by_two_key(NetID netid,string key,string key1,string val);

	//查询数据
	void read(NetID netid,string sql);
	void thread_read();
	void read_callback(int ret,NetID netid);
	
	//写入数据
	bool write(NetID netid,string sql);
	void thread_write();
	void write_callback(int ret,NetID netid);


	void log(MYSQL_RES * result);
private:
	MYSQL mysql_;
	uint32_t sql_read_count_;
	uint32_t sql_write_count_;

	map<uint32_t,SQLID_ptr> read_sql_list_;
	map<uint32_t,SQLID_ptr> write_sql_list_;
	//读写操作线程
	boost::shared_ptr<boost::thread> mysql_read_ptr_; 
	boost::shared_ptr<boost::thread> mysql_write_ptr_; 
};

#define mysqlSrv mysqlmanager::get_instance()

#endif

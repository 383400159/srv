#pragma once
#ifndef IMYSQLDB_H
#define IMYSQLDB_H

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
#include "IMysqlCallBack.h"
#include "IModule.h"


#pragma comment(lib,"libmysql.lib") 
using namespace std;
typedef	unsigned int		NetID;
struct SQLID
{
	NetID netid;
	string sql;
};
typedef boost::shared_ptr<SQLID> SQLID_ptr;
class IMysqlDB : public IModule
{
public:
	IMysqlDB(void);
	~IMysqlDB(void);
	//�������ݿ�
	int Init();
	int Start();
	int Update();
	int Stop();
	int Release();
	void Free();
	



	//�û���ȡ���� 
	void get_data_by_key(NetID netid,string key);
	void get_data_by_two_key(NetID netid,string key,string key1="");

	//�û�д�뺯��
	void write_data_by_key(NetID netid,string key,string val);
	void write_data_by_two_key(NetID netid,string key,string key1,string val);

	//��ѯ����
	void read(NetID netid,string sql);
	void thread_read();
	void read_callback(int ret,NetID netid);
	
	//д������
	bool write(NetID netid,string sql);
	void thread_write();
	void write_callback(int ret,NetID netid);


	void log(MYSQL_RES * result);
private:
	MYSQL mysql_;
	IMysqlCallBack* call_back_;
	uint32_t sql_read_count_;
	uint32_t sql_write_count_;

	map<uint32_t,SQLID_ptr> read_sql_list_;
	map<uint32_t,SQLID_ptr> write_sql_list_;
	//��д�����߳�
	boost::shared_ptr<boost::thread> mysql_read_ptr_; 
	boost::shared_ptr<boost::thread> mysql_write_ptr_; 


};


#endif

#pragma once
#ifndef MYSQLMODULE_H
#define MYSQLMODULE_H

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
#pragma comment(lib,"libmysql.lib") 
using namespace std;
class MysqlModule
{
public:
	MysqlModule(void);
	~MysqlModule(void);
	//�������ݿ�
	bool start();

	//��ѯ����
	bool query();
	//д������
	bool write();


	void log(MYSQL_RES * result);
private:
	MYSQL mysql_;
};
#endif

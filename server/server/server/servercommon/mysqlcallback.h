#pragma once
#ifndef MYSQLCALLBACK_H
#define MYSQLCALLBACK_H

#include<iostream>
#include "singleton.h"

typedef	unsigned int		NetID;
using namespace std;
class mysqlcallback : public Singleton<mysqlcallback> 
{
public:
	mysqlcallback(void);
	~mysqlcallback(void);

	void read_callback(int ret,NetID netid);
	void write_callback(int ret,NetID netid);
};
#define mysqlCallBack mysqlcallback::get_instance()
#endif

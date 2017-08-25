#pragma once
#ifndef IMYSQLCALLBACK_H
#define IMYSQLCALLBACK_H

#include<iostream>

typedef	unsigned int		NetID;
using namespace std;
class IMysqlCallBack 
{
public:
	IMysqlCallBack(void);
	~IMysqlCallBack(void);

	void read_callback(int ret,NetID netid);
	void write_callback(int ret,NetID netid);
};

#endif

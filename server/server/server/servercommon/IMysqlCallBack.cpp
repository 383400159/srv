#include "IMysqlCallBack.h"


IMysqlCallBack::IMysqlCallBack(void)
{
}


IMysqlCallBack::~IMysqlCallBack(void)
{
}


void IMysqlCallBack::read_callback(int ret,NetID netid)
{
	cout<<"mysqlcallback read_callback ret:"<<ret<<" netid:"<<netid<<endl;

}
void IMysqlCallBack::write_callback(int ret,NetID netid)
{
	cout<<"mysqlcallback write_callback ret:"<<ret<<" netid:"<<netid<<endl;
}
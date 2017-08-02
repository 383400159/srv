#include "mysqlcallback.h"


mysqlcallback::mysqlcallback(void)
{
}


mysqlcallback::~mysqlcallback(void)
{
}


void mysqlcallback::read_callback(int ret,NetID netid)
{
	cout<<"mysqlcallback read_callback ret:"<<ret<<" netid:"<<netid<<endl;

}
void mysqlcallback::write_callback(int ret,NetID netid)
{
	cout<<"mysqlcallback write_callback ret:"<<ret<<" netid:"<<netid<<endl;
}
#include "MysqlModule.h"


MysqlModule::MysqlModule(void)
{
	//
	mysql_init(&mysql_);
	mysql_options(&mysql_,MYSQL_SET_CHARSET_NAME,"utf8");
	
}


MysqlModule::~MysqlModule(void)
{
	mysql_close(&mysql_);//断开连接
}


bool MysqlModule::start()
{
	const char user[] = "root";         //username
	const char pswd[] = "p925f990";         //password
	const char host[] = "192.168.1.71";    //or"127.0.0.1"
	const char table[] = "mysql";        //database
	unsigned int port = 3306;           //server port        
	auto ret = mysql_real_connect(&mysql_,host,user,pswd,table,port,NULL,CLIENT_MULTI_STATEMENTS);
	if(ret)
	{
		cout<<"connect succeed!"<<endl;
		mysql_query(&mysql_, "SET NAMES GBK"); //设置编码格式,否则在cmd下无法显示中文
		query();
		return true;
	}
	else
	{
		cout<<"connect fail!"<<endl;
		return false;
	}
}


bool MysqlModule::query()
{   
	const char * sql = "select * from test";
	auto res=mysql_real_query(&mysql_,sql,strlen(sql));//查询
	if(!res)
	{
		auto result_=mysql_store_result(&mysql_);//保存查询到的数据到result
		if(result_)
		{
			this->log(result_);
			if(result_!=NULL) mysql_free_result(result_);//使用完以后 释放结果资源
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		cout<<"query sql failed!"<<endl;
		return false;
	}
}

//写入数据
bool MysqlModule::write()
{
	const char * sql = "update test set degree=888 where id=1 ";
	auto ret=mysql_real_query(&mysql_,sql,strlen(sql));//查询
	if (ret)
	{
	}
	return true;
}


void MysqlModule::log(MYSQL_RES * result)
{
	MYSQL_ROW sql_row;
	MYSQL_FIELD *fd;
	char column[32][32];
	int i,j;
	cout<<"number of result: "<<(unsigned long)mysql_num_rows(result)<<endl;
	for(i=0;fd=mysql_fetch_field(result);i++)//获取列名
	{
		strcpy(column[i],fd->name);
	}
	j=mysql_num_fields(result);
	for(i=0;i<j;i++)
	{
		printf("%s\t",column[i]);
	}
	printf("\n");
	while(sql_row=mysql_fetch_row(result))//获取具体的数据
	{
		for(i=0;i<j;i++)
		{
			printf("%s\t",sql_row[i]);
		}
		printf("\n");
	}
}






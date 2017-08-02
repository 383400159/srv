#include "mysqlmanager.h"
#include "mysqlcallback.h"

mysqlmanager::mysqlmanager(void)
{
	//
	mysql_init(&mysql_);
	mysql_options(&mysql_,MYSQL_SET_CHARSET_NAME,"utf8");
	sql_read_count_ = 0;
	mysql_read_ptr_.reset();
	mysql_write_ptr_.reset();
	mysql_write_ptr_.use_count();
}


mysqlmanager::~mysqlmanager(void)
{
	mysql_close(&mysql_);//断开连接
	auto a = mysql_read_ptr_.use_count();
	auto b = mysql_write_ptr_.use_count();
	if (mysql_read_ptr_.use_count()>0)
	{
		mysql_read_ptr_->join();
	}
	if (mysql_write_ptr_.use_count()>0)
	{
		mysql_write_ptr_->join();
	}

}


bool mysqlmanager::start()
{
	const char user[] = "root";         //username
	const char pswd[] = "p925f990";         //password
	const char host[] = "192.168.1.71";    //or"127.0.0.1"
	const char table[] = "mysql";        //database
	unsigned int port = 3306;           //server port        
	auto ret = mysql_real_connect(&mysql_,host,user,pswd,table,port,NULL,0);
	if(ret)
	{
		cout<<"connect succeed!"<<endl;
		mysql_query(&mysql_, "SET NAMES GBK"); //设置编码格式,否则在cmd下无法显示中文

		return true;
	}
	else
	{
		cout<<"connect fail!"<<endl;
		return false;
	}
}


void mysqlmanager::read(NetID netid,string sql)
{	
	//消息唯一号
	sql_read_count_ ++ ;
	if (sql_read_count_>400000000)
	{
		sql_read_count_ = 1;
	}

	//加入消息队列
	SQLID_ptr qv(new SQLID());
	qv->netid = netid;
	qv->sql = sql;
	read_sql_list_.insert(std::make_pair(sql_read_count_,qv));

	
	//触发第一次线程
	if (read_sql_list_.size()==1)
	{
		//启动查询线程
		mysql_read_ptr_.reset(new boost::thread(boost::bind(&mysqlmanager::thread_read, this)));
	}
}


void mysqlmanager::thread_read()
{

	if (read_sql_list_.size()>0 )
	{
		auto it = read_sql_list_.begin();
		SQLID_ptr val =  it->second;
		NetID netid = val->netid;
		string sql = val->sql;
		const char * send = sql.c_str();
		cout<< send << " - "<< it->first<<endl;
		auto res=mysql_real_query(&mysql_,send,strlen(send));//查询
		if(!res)
		{
			auto result_=mysql_store_result(&mysql_);//保存查询到的数据到result
			if(result_)
			{
				this->log(result_);
				mysqlCallBack.read_callback(0,netid);
				if(result_!=NULL) mysql_free_result(result_);//使用完以后 释放结果资源
			}
			else
			{
				mysqlCallBack.read_callback(1,netid);
			}
		}
		else
		{
			mysqlCallBack.read_callback(2,netid);
			//cout<<"query sql failed!"<<endl;
		}
		
		//删除查询
		read_sql_list_.erase(it);
		if (read_sql_list_.size()>0)
		{
			mysql_read_ptr_.reset(new boost::thread(boost::bind(&mysqlmanager::thread_read, this)));
		}
	}
}

void mysqlmanager::read_callback(int ret,NetID netid)
{
	cout<<"query_callback ret:"<<ret<<" netid:"<<netid<<endl;
}
//写入数据
bool mysqlmanager::write(NetID netid,string sql)
{

	//消息唯一号
	sql_write_count_ ++ ;
	if (sql_write_count_>400000000)
	{
		sql_write_count_ = 1;
	}

	//加入消息队列
	SQLID_ptr qv(new SQLID());
	qv->netid = netid;
	qv->sql = sql;
	write_sql_list_.insert(std::make_pair(sql_write_count_,qv));


	//触发第一次线程
	if (write_sql_list_.size()==1)
	{
		//启动查询线程
		mysql_write_ptr_.reset(new boost::thread(boost::bind(&mysqlmanager::thread_write, this)));
	}

	return true;
}



void mysqlmanager::thread_write()
{

	if (write_sql_list_.size()>0 )
	{
		auto it = write_sql_list_.begin();
		SQLID_ptr val =  it->second;
		NetID netid = val->netid;
		string sql = val->sql;
		const char * send = sql.c_str();
		cout<< send << " - "<< it->first<<endl;
		auto res=mysql_real_query(&mysql_,send,strlen(send));//查询
		if(!res)
		{
			mysqlCallBack.write_callback(0,netid);
		}
		else
		{
			mysqlCallBack.write_callback(1,netid);
		}

		//删除查询
		write_sql_list_.erase(it);
		if (write_sql_list_.size()>0)
		{
			mysql_write_ptr_.reset(new boost::thread(boost::bind(&mysqlmanager::thread_write, this)));
		}
	}
}

void mysqlmanager::write_callback(int ret,NetID netid)
{
	cout<<"write_callback ret:"<<ret<<" netid:"<<netid<<endl;
}


void mysqlmanager::get_data_by_key(NetID netid,string key)
{
	string read_sql = "select * from keydata where ";
	read_sql = read_sql + " pkey = " + '"' + key + '"';
	this->read(netid,read_sql);
}


void mysqlmanager::get_data_by_two_key(NetID netid,string key,string key1)
{
	string read_sql = "select * from twokeydata where ";
	read_sql = read_sql + " pkey = " + '"' + key + '"';
	if (key1!="")
	{
		read_sql = read_sql + " and pkey1 = " + '"' + key1 + '"';
	}
	this->read(netid,read_sql);
}

void mysqlmanager::write_data_by_key(NetID netid,string key,string val)
{
	//insert into keydata(pkey,pval) values("ssss","yyyy")  on DUPLICATE KEY UPDATE pval = "ttttst"
	
	string write_sql = "insert into keydata(pkey,pval) ";
	write_sql = write_sql + " values(" + '"' + key + '"' + "," + '"' + val + '"'+ ")" ;
	write_sql = write_sql + " on DUPLICATE KEY UPDATE pval = " + '"' + val + '"';
	this->write(netid,write_sql);
}
void mysqlmanager::write_data_by_two_key(NetID netid,string key,string key1,string val)
{
	string write_sql = "insert into twokeydata(pkey,pkey1,pval) ";
	write_sql = write_sql + " values(" + '"' + key + '"' + ","+ '"' + key1 + '"' + "," + '"' + val + '"'+ ")" ;
	write_sql = write_sql + " on DUPLICATE KEY UPDATE pval = " + '"' + val + '"';
	this->write(netid,write_sql);
}

void mysqlmanager::log(MYSQL_RES * result)
{
	MYSQL_ROW sql_row;
	MYSQL_FIELD *fd;
	char column[32][32];
	int i,j;
	//cout<<"number of result: "<<(unsigned long)mysql_num_rows(result)<<endl;
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






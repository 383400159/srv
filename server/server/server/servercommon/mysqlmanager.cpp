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
	mysql_close(&mysql_);//�Ͽ�����
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
		mysql_query(&mysql_, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����

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
	//��ϢΨһ��
	sql_read_count_ ++ ;
	if (sql_read_count_>400000000)
	{
		sql_read_count_ = 1;
	}

	//������Ϣ����
	SQLID_ptr qv(new SQLID());
	qv->netid = netid;
	qv->sql = sql;
	read_sql_list_.insert(std::make_pair(sql_read_count_,qv));

	
	//������һ���߳�
	if (read_sql_list_.size()==1)
	{
		//������ѯ�߳�
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
		auto res=mysql_real_query(&mysql_,send,strlen(send));//��ѯ
		if(!res)
		{
			auto result_=mysql_store_result(&mysql_);//�����ѯ�������ݵ�result
			if(result_)
			{
				this->log(result_);
				mysqlCallBack.read_callback(0,netid);
				if(result_!=NULL) mysql_free_result(result_);//ʹ�����Ժ� �ͷŽ����Դ
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
		
		//ɾ����ѯ
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
//д������
bool mysqlmanager::write(NetID netid,string sql)
{

	//��ϢΨһ��
	sql_write_count_ ++ ;
	if (sql_write_count_>400000000)
	{
		sql_write_count_ = 1;
	}

	//������Ϣ����
	SQLID_ptr qv(new SQLID());
	qv->netid = netid;
	qv->sql = sql;
	write_sql_list_.insert(std::make_pair(sql_write_count_,qv));


	//������һ���߳�
	if (write_sql_list_.size()==1)
	{
		//������ѯ�߳�
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
		auto res=mysql_real_query(&mysql_,send,strlen(send));//��ѯ
		if(!res)
		{
			mysqlCallBack.write_callback(0,netid);
		}
		else
		{
			mysqlCallBack.write_callback(1,netid);
		}

		//ɾ����ѯ
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
	for(i=0;fd=mysql_fetch_field(result);i++)//��ȡ����
	{
		strcpy(column[i],fd->name);
	}
	j=mysql_num_fields(result);
	for(i=0;i<j;i++)
	{
		printf("%s\t",column[i]);
	}
	printf("\n");
	while(sql_row=mysql_fetch_row(result))//��ȡ���������
	{
		for(i=0;i<j;i++)
		{
			printf("%s\t",sql_row[i]);
		}
		printf("\n");
	}
}






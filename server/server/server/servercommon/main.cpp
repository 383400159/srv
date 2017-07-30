#include <iostream>
//#include "INetworkModule.h"

#include "mysql.h"
using namespace std;
#pragma comment(lib,"libmysql.lib") 
int main()
{
	const char user[] = "root";         //username
	const char pswd[] = "p925f990";         //password
	const char host[] = "192.168.1.110";    //or"127.0.0.1"
	const char table[] = "mysql";        //database
	unsigned int port = 3306;           //server port        
	MYSQL myCont;
	MYSQL_RES *result;
	MYSQL_ROW sql_row;
	MYSQL_FIELD *fd;
	char column[32][32];
	int res;
	mysql_init(&myCont);
	if(0==mysql_options(&myCont,MYSQL_SET_CHARSET_NAME,"utf8"))//�����ַ���
	{
		cout << "�����ַ����ɹ�\n\n" <<endl;
	}
	auto ret = mysql_real_connect(&myCont,host,user,pswd,table,port,NULL,CLIENT_MULTI_STATEMENTS);
	if(ret)
	{
		cout<<"connect succeed!"<<endl;
		mysql_query(&myCont, "SET NAMES GBK"); //���ñ����ʽ,������cmd���޷���ʾ����
		res=mysql_query(&myCont,"select * from samples");//��ѯ
		if(!res)
		{
			result=mysql_store_result(&myCont);//�����ѯ�������ݵ�result
			if(result)
			{
				int i,j;
				cout<<"number of result: "<<(unsigned long)mysql_num_rows(result)<<endl;
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
						printf("%s\n",sql_row[i]);
					}
					printf("\n");
				}
			}
		}
		else
		{
			cout<<"query sql failed!"<<endl;
		}
	}
	else
	{
		cout<<"connect failed!"<<endl;
	}
	if(result!=NULL) mysql_free_result(result);//�ͷŽ����Դ
	mysql_close(&myCont);//�Ͽ�����
	return 0;
}


/*
int main(int argc, char *argv[])
{

	std::cout<<"start--------------------------------"<<std::endl;
	//����������֮�����Ϣ
	INetworkModule* server_net_work = new INetworkModule();
	server_net_work->Listen(7001,5);

	//�����ͻ�����Ϣ
	INetworkModule* client_net_work = new INetworkModule();
	client_net_work->Listen(19001,5);

	std::cout<<"test"<<std::endl;
	getchar(); 
	return 0;
}
*/


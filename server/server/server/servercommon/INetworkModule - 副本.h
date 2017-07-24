#pragma once

#ifndef INETWORKMODULE_H
#define INETWORKMODULE_H
#pragma comment( lib, "ws2_32.lib") 

#ifndef WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
//#include <thread>
#include <vector>
#include <map>
static const char NETWORK_MODULE[] = "NetworkModule";

typedef	unsigned int		NetID;
typedef unsigned int		IP;
typedef unsigned short		Port;

class IEngineNetCallback
{
public:
	 IEngineNetCallback();
	 ~IEngineNetCallback();

	/*
	 ��������acceptʱ��ص��ú���
	 @netid			�����ӵ�netid
	 @ip			Զ�˵�ַ��������
	 @port			Զ�˶˿ں�
	*/
	void OnAccept(Port listen_port, NetID netid, IP ip, Port port);

	/*
	 ���յ�������Ϣʱ�ص��ú���
	 @netid			������Ϣ��Դ��netid
	 @data			��������
	 @length		���ݳ���
	*/
	void OnRecv(NetID netid, const char *data, int length);

	/*
	 ��������Ͽ���ʱ��ص��ú���
	 @netid			�Ͽ����������ӵ�netid
	*/
	void OnDisconnect(NetID netid);

	/*
	 �������첽ConnectAsyn������غ�ص�
	 @result		���ӽ���Ƿ�ɹ�
	 @handle		ConnectAsynʱ���������handle
	 @netid			���ӳɹ�ʱ��netidʱ�������ӵ������id
	 @ip			����ip
	 @port			����port
	*/
	void OnConnect(bool result, int handle, NetID netid, IP ip, Port port);
};

class INetworkModule 
{
public:
	INetworkModule();
	~INetworkModule();
public:
	 int Init();
	 int Start();
	 int Update();
	 int Stop();
	 int Release();
	 void Free();

	/*
	 ע��������Ϣ�ص�
	 @callback		����ص�
	*/
	 void RegisterCallback(IEngineNetCallback * callback);

	/*
	 �����������
	 @port			Ҫ�����Ķ˿ں�
	 @backlog		������İ뿪���ӵ�������
	 @netid_out		out�����������ɹ���ò���Ϊ���������netid��ע�⣬���ټ������ʱʹ��Disconnect������������netid��Ϊ�������
	 @ip_bind		listen ʱ�İ�socket �� ip�����Ϊ0��󶨵�INADDR_ANY
	 @Return		�Ƿ�����ɹ�
	*/
	 bool Listen(Port port, int backlog, NetID *netid_out=0, const char *ip_bind=0);

	/*
	 ������������
	 @ip			Ҫ���ӵ�Զ�˵�ַ
	 @port			Ҫ���ӵ�Զ�˶˿ں�
	 @netid_out		out���������ӳɹ�ʱnetid_outΪ�������ӵ�����id��
	 @time_out		�����������Ӷ����ĳ�ʱʱ�䣬��λ���룬Ĭ��Ϊ3000
	 @Return		�Ƿ����ӳɹ�
	*/
	 bool Connect(const char *ip, Port port, NetID *netid_out, unsigned long time_out=3000);

	/*
	 ������������
	 @ip			Ҫ���ӵ�Զ�˵�ַ��������
	 @port			Ҫ���ӵ�Զ�˶˿ں�
	 @netid_out		out���������ӳɹ�ʱnetid_outΪ�������ӵ�����id��
	 @time_out		�����������Ӷ����ĳ�ʱʱ�䣬��λ���룬Ĭ��Ϊ3000
	 @Return		�Ƿ����ӳɹ�
	*/
	 bool Connect(IP ip, Port port, NetID *netid_out, unsigned long time_out=3000);

	/*
	 �첽�����������ӣ����ӽ���ڻص�OnConnect�еõ�
	 @ip			Ҫ���ӵ�Զ�˵�ַ
	 @port			Ҫ���ӵ�Զ�˶˿ں�
	 @handle		out��������ΪOnConnect�ص�ʱ��һ����ʶ
	 @time_out		�����������Ӷ����ĳ�ʱʱ�䣬��λ���룬Ĭ��Ϊ3000
	 @Return		�ύ�첽���������Ƿ�ɹ�
	*/
	 bool ConnectAsyn(const char *ip, Port port, int *handle, unsigned long time_out=3000);

	/*
	 �첽�����������ӣ����ӽ���ڻص�OnConnect�еõ�
	 @ip			Ҫ���ӵ�Զ�˵�ַ��������
	 @port			Ҫ���ӵ�Զ�˶˿ں�
	 @handle		out��������ΪOnConnect�ص�ʱ��һ����ʶ
	 @time_out		�����������Ӷ����ĳ�ʱʱ�䣬��λ���룬Ĭ��Ϊ3000
	 @Return		�ύ�첽���������Ƿ�ɹ�
	*/
	 bool ConnectAsyn(IP ip, Port port, int *handle, unsigned long time_out=3000);

	/*
	 ������Ϣ����ע�⣬�ú����������������Ѿ��ɹ����ͣ����ǽ���Ϣ�ŵ������Ͷ������첽����
	 ���ĳ�����ӶϿ��ص�disconnectʱ���п��ܵ�����Ϣ����ʧ
	 �ú�������Ϣ������˳�����б�֤��
	 @neiid			����Ŀ�������id
	 @data			���͵���Ϣ��
	 @length		��Ϣ������
	 @Return		�Ƿ�ɹ��ŵ������Ͷ�����
	*/
	 bool Send(NetID netid, const char *data, unsigned int length);

	/*
	 �Ͽ�ĳ���ӣ����ԶϿ����������׽������ڵ���������
	 ע�⣬�ú�������ʱ��������������Ѿ��Ͽ������ǽ������ӱ��Ϊ���Ͽ�
	 �����Ͽ�����Ϊ����IEngineNetCallback::OnDisconnect��֤��
	 @netid			Ҫ�Ͽ�������id
	*/
	 void Disconnect(NetID netid);


private:
	SOCKET SocketID;

	struct sockaddr_in init_server;
	std::map<SOCKET,IEngineNetCallback*> AcceptMapList;
};

#endif

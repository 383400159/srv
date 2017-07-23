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
	 单有连接accept时则回调该函数
	 @netid			该连接的netid
	 @ip			远端地址，主机序
	 @port			远端端口号
	*/
	void OnAccept(Port listen_port, NetID netid, IP ip, Port port);

	/*
	 当收到网络消息时回调该函数
	 @netid			网络消息来源的netid
	 @data			网络数据
	 @length		数据长度
	*/
	void OnRecv(NetID netid, const char *data, int length);

	/*
	 当有网络断开的时候回调该函数
	 @netid			断开的网络连接的netid
	*/
	void OnDisconnect(NetID netid);

	/*
	 当调用异步ConnectAsyn结果返回后回调
	 @result		连接结果是否成功
	 @handle		ConnectAsyn时的输出参数handle
	 @netid			连接成功时，netid时所得连接的网络层id
	 @ip			连接ip
	 @port			连接port
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
	 注册网络消息回调
	 @callback		网络回调
	*/
	 void RegisterCallback(IEngineNetCallback * callback);

	/*
	 创建监听句柄
	 @port			要监听的端口号
	 @backlog		所允许的半开连接的最大个数
	 @netid_out		out参数，监听成功则该参数为监听句柄的netid。注意，销毁监听句柄时使用Disconnect函数，并将该netid作为传入参数
	 @ip_bind		listen 时的绑定socket 的 ip，如果为0则绑定到INADDR_ANY
	 @Return		是否监听成功
	*/
	 bool Listen(Port port, int backlog, NetID *netid_out=0, const char *ip_bind=0);

	/*
	 建立网络连接
	 @ip			要连接的远端地址
	 @port			要连接的远端端口号
	 @netid_out		out参数，连接成功时netid_out为建立连接的网络id号
	 @time_out		建立网络连接动作的超时时间，单位毫秒，默认为3000
	 @Return		是否连接成功
	*/
	 bool Connect(const char *ip, Port port, NetID *netid_out, unsigned long time_out=3000);

	/*
	 建立网络连接
	 @ip			要连接的远端地址，主机序
	 @port			要连接的远端端口号
	 @netid_out		out参数，连接成功时netid_out为建立连接的网络id号
	 @time_out		建立网络连接动作的超时时间，单位毫秒，默认为3000
	 @Return		是否连接成功
	*/
	 bool Connect(IP ip, Port port, NetID *netid_out, unsigned long time_out=3000);

	/*
	 异步建立网络连接，连接结果在回调OnConnect中得到
	 @ip			要连接的远端地址
	 @port			要连接的远端端口号
	 @handle		out参数，作为OnConnect回调时的一个标识
	 @time_out		建立网络连接动作的超时时间，单位毫秒，默认为3000
	 @Return		提交异步连接任务是否成功
	*/
	 bool ConnectAsyn(const char *ip, Port port, int *handle, unsigned long time_out=3000);

	/*
	 异步建立网络连接，连接结果在回调OnConnect中得到
	 @ip			要连接的远端地址，主机序
	 @port			要连接的远端端口号
	 @handle		out参数，作为OnConnect回调时的一个标识
	 @time_out		建立网络连接动作的超时时间，单位毫秒，默认为3000
	 @Return		提交异步连接任务是否成功
	*/
	 bool ConnectAsyn(IP ip, Port port, int *handle, unsigned long time_out=3000);

	/*
	 发送消息包，注意，该函数并不代表数据已经成功发送，而是将消息放到待发送队列中异步发送
	 如果某个连接断开回调disconnect时，有可能导致消息包丢失
	 该函数对消息包发送顺序是有保证的
	 @neiid			发送目标的网络id
	 @data			发送的消息包
	 @length		消息包长度
	 @Return		是否成功放到待发送队列中
	*/
	 bool Send(NetID netid, const char *data, unsigned int length);

	/*
	 断开某连接，可以断开包括监听套接字在内的所有连接
	 注意，该函数返回时并不代表该连接已经断开，而是将该连接标记为待断开
	 真正断开的行为是由IEngineNetCallback::OnDisconnect保证的
	 @netid			要断开的网络id
	*/
	 void Disconnect(NetID netid);


private:
	SOCKET SocketID;

	struct sockaddr_in init_server;
	std::map<SOCKET,IEngineNetCallback*> AcceptMapList;
};

#endif

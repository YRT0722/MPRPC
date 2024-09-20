#pragma once
#include"google/protobuf/service.h"
#include<muduo/net/TcpServer.h>//TCP服务
#include<muduo/net/EventLoop.h>//IO
#include<muduo/net/InetAddress.h>//地址端口
#include<muduo/net/TcpConnection.h>
#include<functional>
#include<google/protobuf/descriptor.h>
#include<string>
#include<iostream>
//专门负责发布rpc服务的网络对象类,储存了网络服务对象、方法以及描述通过结构体和hashmap

class RpcProvider
{
    public:
    //发布rpc方法的函数接口
    void NotifyService(google::protobuf::Service *service);
    //启动rpc服务，提供rpc方法的远程调用
    void run();

private:
//ServiceInfo服务器对象内容，m_serveMap不同服务对象
    struct ServiceInfo
    {
        google::protobuf::Service* m_service;//服务对象
        std::unordered_map<std::string,const google::protobuf::MethodDescriptor *>m_methodMap;//服务方法

    };

    std::unordered_map<std::string,ServiceInfo>m_serveMap;
    
    std::unique_ptr<muduo::net::TcpServer>m_tcpserverPtr;
    muduo::net::EventLoop m_eventLoop;
    //新的连接回调
    void OnConnection(const muduo::net::TcpConnectionPtr & conn);
    //读写时间回调
    void OnMessage(const muduo::net::TcpConnectionPtr& conn,
                            muduo::net::Buffer* buffer,
                            muduo::Timestamp receiveTime);
    //closure的回调操作，用于序列化rpc的响应和发送
    void SendRpcResponse(const muduo::net::TcpConnectionPtr&, google::protobuf::Message*);
    

};

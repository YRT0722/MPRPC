#include<iostream>
#include"mprpcapplication.h"
#include<user.pb.h>
#include"mprpcchannel.h"
#include<mprpccon>

#include<google/protobuf/service.h>

int main(int argc,char **argv)
{
    //初始化框架，绑定IP和端口
    MprpcApplication::Init(argc,argv);

    //远程调用rpc
    fixbug::UserServiceRpc_Stub stud(new MprpcChannel());//通过Rpcchannel对象完成构造，类内初始化一个MprpcChannel对象
    // fixbug::LoginRequest request;
    // request.set_name("zhan san");
    // request.set_pwd("123456");

    // //rpc方法响应
    // fixbug::LoginResponse response;//通过rpcchannel的回调函数实现调用函数
   

    // //发起调用
    // stud.Login(nullptr,&request,&response,nullptr);

    // //调用响应
    // if(response.result().errcode()==0)
    // {
    //     std::cout<<"rpc login successful: "<<response.success()<<std::endl;
    // }
    // else
    // {
    //     std::cout<<"rpc login err"<<response.result().errmsg()<<std::endl;
    // }
    // std::cout<<"调用结束"<<std::endl;

    fixbug::RegisterRequest req;
    req.set_id(99);
    req.set_name("qwe");
    req.set_pwd("12344");
    fixbug::RegisterResponse rsp;

    stud.Register(nullptr,&req,&rsp,nullptr);
      if(rsp.result().errcode()==0)
    {
        std::cout<<"rpc login successful: "<<rsp.success()<<std::endl;
    }
    else
    {
        std::cout<<"rpc login err"<<rsp.result().errmsg()<<std::endl;
    }
    std::cout<<"调用结束"<<std::endl;
 
 

    return 0;
}

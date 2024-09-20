#include<iostream>
#include<string>
#include"user.pb.h"
#include"rpcprovider.h"
#include"mprpcapplication.h"
using namespace std;
using namespace fixbug;

//通过对服务对象挂到网络上进行通信，注册一个服务器，储存了一些方法和数据，所有数据传输通过protobuf，必须使用定义消息类型
class UserService: public UserServiceRpc//使用rpc服务发布端，发布端和消费端都是同一个数据对象和方法
{
    public:
    bool Login(string name,string pwd)
    {
        cout<<"name:"<<name<<endl;
        cout<<"pwd:"<<pwd<<endl;
        return false;
    }
    bool Register(uint32_t id,string name,string pwd)
    {
        cout<<"id: "<<id<<" name: "<<name<<" pwd: "<<pwd<<endl;
        return true;
    }
    //重写基类是一个服务端口，是一个服务框架通过重写来实现业务，框架来接收参数，我使用这些参数来实现本地业务
    void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,
                       ::fixbug::LoginResponse* response,
                       ::google::protobuf::Closure* done)
        {
            std::cout<<"执行了"<<std::endl;
            //框架给了执行业务调用所需的方法和参数
            string name = request->name();
            string pwd = request->pwd();
            //执行业务
            bool login_result=Login(name,pwd);
            //把响应写入 错误码，返回值
            Resultcode *code=response->mutable_result();
            code->set_errcode(1);
            code->set_errmsg("");
            response->set_success(login_result);
            //执行回调 执行响应对象数据的序列化和网络发送（都是框架来完成）
            done->Run();
        }
     void Register(google::protobuf::RpcController* controller,
                       const ::fixbug::RegisterRequest* request,
                       ::fixbug::RegisterResponse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t id=request->id();
        string name=request->name();
        string pwd=request->pwd();

        bool ret=Register(id,name,pwd);
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        response->set_success(ret);
        done->Run();
    }
};

int main(int argc,char** argv)
{
    //调用框架初始化操作 provider -i config.conf读取配置文件
    MprpcApplication::Init(argc,argv);

    //provider是一个rpc网络服务对象，把UserService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService());
    //启动一个rpc服务发布节点，Run以后进入阻塞状态，等待远程的Rpc调用请求
    provider.run();
    return 0;
}
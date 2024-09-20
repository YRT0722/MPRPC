#include"mprpcchannel.h"
#include<string>
#include<iostream>
#include"rpcheader.pb.h"
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<mprpcapplication.h>
#include<unistd.h>
#include<mprpcchannel.h>
#include<zookeeperutil.h>

//实现将服务对象、服务方发方法和以及所需参数（请求参数），再将回发参数读取，反序列化后弄到reponse对象上，都是通过对象携带参数进行传递
void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                    google::protobuf::RpcController* controller,
                    const google::protobuf::Message* request,
                    google::protobuf::Message* response,
                    google::protobuf::Closure* done)
{
    const google::protobuf::ServiceDescriptor *sd=method->service();
    const std::string service_name=sd->name();
    std::string method_name=method->name();
    uint32_t args_size=0;
    std::string args_str;//参数
    if(request->SerializeToString(&args_str))
    {
        args_size=args_str.size();

    }
    else{
        std::cout<<"serialist request error!"<<std::endl;
        controller->SetFailed("serialist request error!");
        return ;

    }
    mprpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);

    uint32_t header_size=0;
    std::string rpc_header_str;//请求头内容
    if(rpcHeader.SerializeToString(&rpc_header_str))
    {
        header_size=rpc_header_str.size();//请求头大小
    }
    else
    {
        std::cout<<"serialist request error!"<<std::endl;
        controller->SetFailed("serialist request error!");
        return ;
    }
    //组织到一起
    std::string send_rpc_str;
    send_rpc_str.insert(0,std::string((char*)&header_size,4));//加入4字节的请求头
    std::cout << "send_rpc_str size: " << send_rpc_str.size() << std::endl;

    send_rpc_str+=rpc_header_str;
    send_rpc_str+=args_str;
    std::cout<<"header_size:"<<header_size<<std::endl;
    std::cout<<"rpc_header_str:"<<rpc_header_str<<std::endl;
    std::cout<<"args_str:"<<args_str<<std::endl;
    std::cout<<"send_rpc_str:"<<send_rpc_str<<std::endl;

    //使用TCP编程完成调用
    int cfd=socket(AF_INET,SOCK_STREAM,0);
    if(cfd==-1)
    {
        std::cout<<"create socket err! errno: "<<errno<<std::endl;
        controller->SetFailed("create socket err!");
        exit(EXIT_FAILURE);
    }
    // std::string ip=MprpcApplication::GetInstance().GetConfig().Load("rpcserveip");
    // uint16_t port=std::atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserveport").c_str());
    //
    //zookeeper
    ZkClient zkCli;
    zkCli.Start();
    //  /UserServiceRpc/Login
    std::string method_path = "/" + service_name + "/" + method_name;
    // 127.0.0.1:8000
    std::string host_data = zkCli.GetData(method_path.c_str());
    if (host_data == "")
    {
        controller->SetFailed(method_path + " is not exist!");
        return;
    }
    int idx = host_data.find(":");
    if (idx == -1)
    {
        controller->SetFailed(method_path + " address is invalid!");
        return;
    }
    std::cout<<"================"<<std::endl;
    std::string ip = host_data.substr(0, idx);
    uint16_t port = atoi(host_data.substr(idx+1, host_data.size()-idx).c_str()); 
    
    struct sockaddr_in service_addr;
    service_addr.sin_family=AF_INET;
    service_addr.sin_port=htons(port);
    //inet_pton(AF_INET,ip.c_str(),&service_addr.sin_addr.s_addr);
    //service_addr.sin_addr.s_addr=inet_addr(ip.c_str());
    if (inet_pton(AF_INET, ip.c_str(), &service_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address or address not supported" << std::endl;
        close(cfd);
        exit(EXIT_FAILURE);
    }
    
    if(connect(cfd,(struct sockaddr*)&service_addr,sizeof(service_addr))==-1)
    {
        std::cout<<"connect err!"<<std::endl;
        controller->SetFailed("connect err!");
        //perror("connect");
        close(cfd);
        exit(EXIT_FAILURE);//不明白为什么退出，他失败后续操作无法继续
    }
    std::cout<<send_rpc_str<<std::endl;
    if(-1==send(cfd,send_rpc_str.c_str(),send_rpc_str.size(),0))
    {
        std::cout<<"send err!"<<std::endl;
        controller->SetFailed("send err!");
         close(cfd);
        return;//不明白为什么return
    }
    std::cout<<"send successful"<<std::endl;
    char buf[1024]={0};
    int buf_size=0;
    if((buf_size=recv(cfd,buf,1024,0))==-1)
    {
         std::cout<<"recv err!"<<std::endl;
         controller->SetFailed("recv err!");
        // perror("recv err");
          close(cfd);
        return;//不明白为什么return
    }
    
    std::string response_str(buf,0,buf_size);//字符构造，用子段标识符来识别反序列时对象信息的位置，在反序列化过程中，protobuf 会解析 tag 和字段值，将数据填充到对象中，子段标号就是对象序列化和反序列时，对象用来确认子段值位置的东西
   //ParseFromString 是基于字符串内容解析的，可能会因为字符串中多余的字符（如末尾的空字符）引起问题。
    if(!response->ParseFromArray(buf,buf_size))
       {std::cout<<"ParseFromString err!"<<std::endl;
        std::cerr << "ParseFromString error! Data size: " << buf_size << std::endl;
        std::cerr << "Received data: " << response_str << std::endl;  // 如果数据较小可以打印出来
        controller->SetFailed("ParseFromString err!");
        close(cfd);
        return;}//不明白为什么return}
    
    close(cfd);
    std::cout<<"已经使用完毕"<<std::endl;
}

    

    


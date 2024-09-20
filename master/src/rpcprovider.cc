#include"rpcprovider.h"
#include"rpcheader.pb.h"
#include"mprpcapplication.h"
#include"zookeeperutil.h"
#include<string>


//发布rpc方法的函数接口 就是服务器接收到信息以后，怎么知道这是方法的信息，传入一个子类服务方法
//通过不同子类对象来实现不同服务对象的调用
void RpcProvider::NotifyService(google::protobuf::Service *service)
{
    ServiceInfo service_info;
  //服务对象的描述信息
  const google::protobuf::ServiceDescriptor *pserviceDesc=service->GetDescriptor();
  //服务器名字
  std::string service_name=pserviceDesc->name();
  std::cout<<"service_name:"<<service_name<<std::endl;
  //服务方法数量
  int methodCnt=pserviceDesc->method_count();
  for(int i=0;i<methodCnt;++i)
  {
    const google::protobuf::MethodDescriptor *MethodDescription= pserviceDesc->method(i);
    std::string method_name=MethodDescription->name();
    service_info.m_methodMap.insert({method_name,MethodDescription});
    std::cout<<"method_name:"<<method_name<<std::endl;
  }
  service_info.m_service=service;
  m_serveMap.insert({service_name,service_info});
}

//启动rpc服务，提供rpc方法的远程调用
void RpcProvider::run(){
    std::string ip=MprpcApplication::GetInstance().GetConfig().Load("rpcserveip");
    uint16_t port=atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserveport").c_str());
    muduo::net::InetAddress address(ip,port);
    //创建TCP服务器
    muduo::net::TcpServer serve(&m_eventLoop,address,"RpcProvider");
    //绑定回调和消息读写回调i，必须绑定对象this指针否则会报错，placeholders::_1占位符
    serve.setConnectionCallback(std::bind(&RpcProvider::OnConnection,this,std::placeholders::_1));
   //成员函数的回调必须依赖对象，所以绑定this指针
   serve.setMessageCallback(std::bind(&RpcProvider::OnMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));
    //设置muduo库的线程数
    serve.setThreadNum(4);
  //心跳时间  zkclient 网络IO/年 1/3心跳时间发送pin消息 重置时间
    ZkClient zkCli;
    zkCli.Start();
    for(auto &sp:m_serveMap)
    {
      std::string service_path="/"+sp.first;
      zkCli.Create(service_path.c_str(), nullptr, 0);
      for(auto &mp:sp.second.m_methodMap)
      {
          std:: string method_path=service_path+"/"+mp.first;
          //zkCli.Create(method_path.c_str(), nullptr,0);
          //服务方法都注册成临时性节点,否者服务器挂了，服务方法还残留就会造成问题
          char method_data[128]={0};
          sprintf(method_data,"%s:%d",ip.c_str(),port);
          std::cout<<method_data<<std::endl;
          zkCli.Create(method_path.c_str(),method_data,strlen(method_data),ZOO_EPHEMERAL);
      }
    }
    std::cout<<"RpcProvide start service at ip:"<<ip<<" port:"<<port<<std::endl;
    serve.start();
    //std::cout<<"1"<<std::endl;
    //这个相当于epoll_wait()
    m_eventLoop.loop();



}
//总的来说读取方法和参数信息，然后绑定回方法的回调函数
//head——size+head_str+
//消息回调,将收到的信息进行反序列化再进行解析，数据对象和数据方法、参数，采用protobuf来进行，头大小+字符串+参数大小


//执行回调reponse的回复
void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response)
{
 
    std::string response_str;
    
    //序列化为二进制数据
    if(response->SerializeToString(&response_str))
    {
        //序列化成功后，通过网络把rpc方法执行的结果发送给rpc的调用方
        //std::cout<<response_str<<std::endl;
        conn->send(response_str);
        std::cout<<"send  response_str successful!"<<response_str<<std::endl;
    }
    else{
      std::cout<<"searialize response_str error!"<<std::endl;
    }
    conn->shutdown();//断开连接
}
//新的连接回调 有服务请求就会建立连接回调
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr & conn)
{
  //std::cout<<"connect successful!"<<std::endl;
  if(!conn->connected())
  {
    //std::cout<<"connect err"<<std::endl;
      conn->shutdown();
      return;
  }
  std::cout<<"connect successful!"<<std::endl;

}

//框架内部通信有自己的数据类型，按照他=给数据类型进行通信
//string insert copy，，可以直接复制内存，将二进制数据存贮在字符中，需要读取字符的地址获取整数的大小
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn,
                            muduo::net::Buffer* buffer,
                            muduo::Timestamp receiveTime)
{
  std::cout<<"到这里了"<<std::endl;
  std::string recv_buf=buffer->retrieveAllAsString();//是字符但是存的是二进制数据
    if (recv_buf.size() < 4)
    {
        std::cout << "接收到的数据不足，无法读取 header_size" <<recv_buf<< std::endl;
        return;
    }
  uint32_t header_size=0;
  recv_buf.copy((char*)&header_size,4,0);

  //读字符
  std::string header_str=recv_buf.substr(4,header_size);
  mprpc::RpcHeader rpcheader;
  std::string service_name;
  std::string method_name;
  uint32_t args_size;
  if(rpcheader.ParseFromString(header_str))
  {
      service_name=rpcheader.service_name();
      method_name=rpcheader.method_name();
      args_size=rpcheader.args_size();
  }
  else
  {
    std::cout<<"rpcheader.ParseFromString err!"<<std::endl;
    return;
  }
  std::string args_str=recv_buf.substr(4+header_size,args_size);
  std::cout<<"==========================="<<std::endl;
  std::cout<<"service_name: "<<service_name<<std::endl;
  std::cout<<"method_name: "<<method_name<<std::endl;
  std::cout<<"args_size: "<<args_size<<std::endl;
  std::cout<<"==========================="<<std::endl;

  auto it=m_serveMap.find(service_name);//通过迭代器访问map键值对内容
  if(it==m_serveMap.end())
  {
    std::cout<<"服务对象不存在"<<std::endl;
  }
  auto mit=it->second.m_methodMap.find(method_name);
  if(mit==it->second.m_methodMap.end())
  {
    std::cout<<"服务对象的服务方法不存在"<<std::endl;
  }
  google::protobuf::Service * service=it->second.m_service;
  const google::protobuf::MethodDescriptor * method=mit->second;
  //fixbug::LoginRequest;
  //根据方法生成对应类型，根据方法描述获取原型对象，动态消息创建，运行时才知道要创建那个示例，更加灵活，每个服务方法都有自己的消息类型
  google::protobuf::Message * request=service->GetRequestPrototype(method).New();
  if(!request->ParseFromString(args_str))
  {
      std::cout<<"request parse err!"<<std::endl;
      return;
  }
  google::protobuf::Message * response=service->GetResponsePrototype(method).New();
  //是一个模板，继承自closure对象，可以根据传入函数自动生成一个cloure对象
  google::protobuf::Closure* done=google::protobuf::NewCallback<RpcProvider,const muduo::net::TcpConnectionPtr&, google::protobuf::Message *>(this,&RpcProvider::SendRpcResponse,conn,response);

  //具体的人干具体的事务,通过服务方法的回调来实现，通过反射机制，动态分析要调用的方法，只重写了Login，回复也需要调用
  service->CallMethod(method,nullptr,request,response,done);

}

//  void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr&conn, google::protobuf::Message*response)
// {
    
// }
                


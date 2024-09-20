#include<iostream>
#include"mprpcapplication.h"
#include<friend.pb.h>
#include"mprpccontroller.h"
#include<mprpcchannel.h>
#include<google/protobuf/service.h>

int main(int argc,char **argv)
{
    //初始化框架，绑定IP和端口
    MprpcApplication::Init(argc,argv);

    //远程调用rpc
    fixbug::UserFriendRpc_Stub stud(new MprpcChannel());//通过Rpcchannel对象完成构造，类内初始化一个MprpcChannel对象
    

    fixbug::GetFriendsListRequest req;
    req.set_userid(99);
    fixbug::GetFriendsListResponse rsp;
    MprpcController ctl;
    stud.GetFrinendsList(&ctl,&req,&rsp,nullptr);
    if(ctl.Failed())
    {
        std::cout<<ctl.ErrorText()<<std::endl;
    }
    else
    {
          if(rsp.result().errcode()==0)
    {
        rsp.friends_size();
        std::cout<<"rpc GetFrinendsList successful: "<<std::endl;
        for(int i=0;i<rsp.friends_size();++i)
        {
            std::cout<<"name: "<<rsp.friends(i)<<std::endl;
        }
    }
    else
    {
        std::cout<<"rpc GetFrinendsList err"<<rsp.result().errmsg()<<std::endl;
    }
    std::cout<<"调用结束"<<std::endl;
    }
    
 
 

    return 0;
}

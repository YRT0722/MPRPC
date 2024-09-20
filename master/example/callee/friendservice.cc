#include<iostream>
#include<string>
#include"friend.pb.h"
#include"rpcprovider.h"
#include"mprpcapplication.h"
#include"logger.h"

class FriendService: public fixbug::UserFriendRpc
{
private:
      
public:
    //这个函数无所谓函数名只是一个实现本地业务的工具
    std::vector<std::string> GetFrinends(uint32_t userid)
    {
            std::cout<<"doing GetFriendList service"<<std::endl;
          std::vector<std::string>res;
            res.push_back("yang");
            res.push_back("rong");
            res.push_back("tao");
            return res;
    }
    //这个才是真正的服务方法
      void GetFrinendsList(google::protobuf::RpcController* controller,
                       const ::fixbug::GetFriendsListRequest* request,
                       ::fixbug::GetFriendsListResponse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t id=request->userid();
        std::vector<std::string>re=GetFrinends(id);
        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        for(std::string& str:re)
        {
            //获取一个指针新条目，你可以修改这个条目
            std::string *p=response->add_friends();//
            //访问p是地址，类型相同才能赋值
            *p=str;
        }
        done->Run();
    }
};



int main(int argc,char ** argv)
{
    LOG_INFO("first log messasge!");
    LOG_ERROR("%s:%s:%d",__FILE__,__FUNCTION__,__LINE__);
    MprpcApplication::Init(argc,argv);
     LOG_INFO("first log messasge!");
    RpcProvider provider;
    provider.NotifyService(new FriendService());
    provider.run();
    return 0;
}
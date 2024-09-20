#include<iostream>
#include"test.pb.h"
#include<string>
using namespace fixbug;
using namespace std;

int main()
{
    // LoginResponse rsp;
    // Resultcode *rc=rsp.mutable_result();
    // rc->set_errcode(1);
    // rc->set_errmsg("登陆出错了");

    GetFriendListsResponse rsp;
    Resultcode* rc=rsp.mutable_result();
    rc->set_errcode(0);
    User *usr1=rsp.add_user();
    usr1->set_name("zhan san");
    usr1->set_age(22);
    usr1->set_sex(User::MAN);

    User *usr2=rsp.add_user();
    usr2->set_name("zhan yan");
    usr2->set_age(21);
    usr2->set_sex(User::MAN);

    cout<<rsp.user_size()<<endl;


    return 0;
}

// int main1()
// {
//     //封装了login请求对象的数据
//     LoginRequest req;
//     req.set_name("zhang san");
//     req.set_pwd("12345");

//     //对象数据序列化
//     string str;
//     if(req.SerializePartialToString(&str))
//     {
//         cout<<str.c_str()<<endl;
//     }

//     //对象数据反序列化
//     LoginRequest reqB;
//     if(reqB.ParseFromString(str))
//     {
//         cout<<reqB.name()<<endl;
//         cout<<reqB.pwd()<<endl;

//     }
//     return 0;
// }
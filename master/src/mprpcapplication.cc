#include"mprpcapplication.h"
#include<iostream>
#include<string>
#include<unistd.h>

MprpcConfig MprpcApplication::m_config;//其他类中使用静态成员变量需要定义静态成员变量

void ShowArgsHelp()
{
    std::cout<<"format: comment -i <configfile>"<<std::endl;
}



void MprpcApplication::Init(int argc,char** argv)
{

    if(argc<2)
    {
        ShowArgsHelp();
        exit(EXIT_FAILURE);
    }
    int opt=0;
    std::string config_file;
    //std::cout<<1<<std::endl;
    while ((opt=getopt(argc,argv,"i:"))!=-1)
    {
        switch (opt)
        {
        case 'i':
            config_file=optarg;
            break;
        case '?':
        std::cout<<"invalid args!"<<std::endl;
        ShowArgsHelp();
        exit(EXIT_FAILURE);
        case ':':
         std::cout<<"need <config_file>"<<std::endl;
         ShowArgsHelp();
        exit(EXIT_FAILURE);
        default:
            break;
        }
    
    }
    m_config.LoadConfig(config_file.c_str());//string转换成c
    std::cout<<"rpcserveip:"<<m_config.Load("rpcserveip")<<std::endl;
    std::cout<<"rpcserveport:"<<m_config.Load("rpcserveport")<<std::endl;

}


MprpcApplication& MprpcApplication::GetInstance()
{
    static MprpcApplication app;
    return app;
}

MprpcConfig& MprpcApplication::GetConfig()
{
    return m_config;
}
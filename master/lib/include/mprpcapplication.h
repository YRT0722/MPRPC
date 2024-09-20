#pragma once
#include"mprpcconfig.h"
//框架初始化的类
class MprpcApplication
{
private:
    /* data */
public:
    static void Init(int argc,char** argv);
    static MprpcApplication& GetInstance();
    static MprpcConfig& GetConfig();
   
private:
    static MprpcConfig m_config;
    MprpcApplication(){};
    MprpcApplication(const MprpcApplication&)=delete;
    MprpcApplication(MprpcApplication&&)=delete;
   
};



#include "mprpcconfig.h"
#include<iostream>
#include<string>
#include<functional>

void MprpcConfig::LoadConfig(const char *config_file)
{
    FILE* pf=fopen(config_file,"r");
    if(pf==nullptr)
    {
        std::cout<<config_file<<" is invalid!"<<std::endl;
        exit(EXIT_FAILURE);
    }
    while (!feof(pf))
    {
        char buf[512];
        fgets(buf,512,pf);

        std::string str_buf(buf);
        Trim(str_buf);

        if(str_buf.empty()||str_buf[0]=='#')continue;//去掉多余空格和注释
        //解析配置项目
        int idx=str_buf.find('=');
        if(idx==-1)continue;

        std::string key;
        std::string value;
        key=str_buf.substr(0,idx);
        Trim(key);
        int edix=str_buf.find('\n');
        value=str_buf.substr(idx+1,edix-idx-1);
        Trim(value);
        m_configMap.insert({key,value});//初始化类成员，可以直接使用

    }
}

void MprpcConfig::Trim(std::string &str_buf)
{
         int idx=str_buf.find_first_not_of(' ');
        if(idx!=-1)str_buf=str_buf.substr(idx,str_buf.size()-idx);
        idx=str_buf.find_last_not_of(' ');
        if(idx!=-1)str_buf=str_buf.substr(0,idx+1);
       

}

std::string MprpcConfig::Load(const std::string&key)
{
    auto it=m_configMap.find(key);
    if(it==m_configMap.end())return  "无数据或未读取到";
    return it->second;
}


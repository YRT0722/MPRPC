#pragma once

#include<string>
#include<unordered_map>

class MprpcConfig
{
    public:
    void LoadConfig(const char *config_file);
    std::string Load(const std::string&key);

private:
    std::unordered_map<std::string,std::string>m_configMap;
    //去掉空格
    void Trim(std::string &str_buf);


};

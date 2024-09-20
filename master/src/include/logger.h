#pragma once
#include"lockqueue.h"

//单例简化接口
enum LogLevel
{
    INFO,//普通消息
    ERROR,//错误消息
};

class Logger
{
public:
    static Logger& GetInstance();

    void SetLogLevel(LogLevel level);//日志级别
    void Log(std::string msg);//内容

    //static Logger;

private:
    int m_loglevel;//记录日志级别 ？？
    //模板类对象
    LockQueue<std::string>m_lockqueue;//类内可以访问
    Logger();
    Logger(const Logger&)=delete;
    Logger(const Logger&&)=delete;
};

//定义宏格式
#define LOG_INFO(logmsgformat,...)\
do\
{\
    Logger &logger=Logger::GetInstance();\
    logger.SetLogLevel(INFO);\
    char c[1024]={0};\
    snprintf(c,1024,logmsgformat,##__VA_ARGS__);\
    logger.Log(c);\
}while (0)

#define LOG_ERROR(logmsgformat,...)\
do\
{\
    Logger &logger=Logger::GetInstance();\
    logger.SetLogLevel(ERROR);\
    char c[1024]={0};\
    snprintf(c,1024,logmsgformat,##__VA_ARGS__);\
    logger.Log(c);\
}while (0)
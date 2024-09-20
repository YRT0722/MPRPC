#include"logger.h"
#include<iostream>



Logger& Logger::GetInstance()
 {
    //这个实例是全局唯一的，静态成员变量是类级别的，类实例只创建一次，并提供全局访问点
    static Logger  logger;
    return logger;
 }

void Logger::SetLogLevel(LogLevel level)
{
    m_loglevel=level;
}
void Logger::Log(std::string msg)
{
    m_lockqueue.Push(msg);
}

 Logger::Logger()
 {
    std::thread writeLogTask(
        [&](){
            for(;;)
            {
                time_t now = time(nullptr);//获取当前时间，传入非nullptr就存储在这个指针上
                tm *nowth=localtime(&now);//转化当前时间
                //创建日志文件
                char filename[1024];
                sprintf(filename,"%d-%d-%d-log.txt",nowth->tm_year+1900,nowth->tm_mon,nowth->tm_mday);
                
                FILE * fp=fopen(filename,"a+");
                if(fp==nullptr)
                {
                    std::cout<<filename<<"打开文件失败"<<std::endl;
                    exit(EXIT_FAILURE);
                }
                std::string msg =  m_lockqueue.Pop();
                char time_str[128]={0};
                sprintf(time_str,"%d:%d:%d:[%s]==>",nowth->tm_hour,
                nowth->tm_min,nowth->tm_min,(m_loglevel==INFO?"INFO":"ERROR"));
                msg.insert(0,time_str);
                msg.push_back('\n');
                fputs(msg.c_str(),fp);
                fclose(fp);
            }
        }
    );
    writeLogTask.detach();
 }
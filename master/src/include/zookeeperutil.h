#pragma once
#include<semaphore.h>
#include<zookeeper/zookeeper.h>
#include<string>

class ZkClient
{
public:
    ZkClient();
    ~ZkClient();
    //启动连接服务
    void Start();
    //在服务器上创建结点,0是永久结点
     void Create(const char *path, const char *data, int datalen, int state=0);
     //根据结点路径获取值
     std::string GetData(const char *path);
private:
//客户端句柄
    zhandle_t *m_zhandle;




};
#pragma once
#include<google/protobuf/service.h>
#include<string>

class MprpcController:public google::protobuf::RpcController 
{
 public:
   MprpcController();
  void Reset() ;
   bool Failed() const;//检查某个操作过程是否出错
  // If Failed() is true, returns a human-readable description of the error.
    std::string ErrorText() const;//输出错误文本
   
    void SetFailed(const std::string& reason) ;//设置错误文本
    // 目前未实现具体的功能
    void StartCancel();
    bool IsCanceled() const;
    void NotifyOnCancel(google::protobuf::Closure* callback) ;
private:
     bool m_failed; // RPC方法执行过程中的状态
    std::string m_errText; // RPC方法执行过程中的错误信息


};
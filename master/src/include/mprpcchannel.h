#pragma once

#include<google/protobuf/service.h>
#include<google/protobuf/message.h>
#include<google/protobuf/descriptor.h>

class MprpcChannel: public google::protobuf::RpcChannel
{
//所有通过rpc调用的方法都在这里，统一做rpc方法的调用
public:
    void CallMethod(const google::protobuf::MethodDescriptor* method,
                    google::protobuf::RpcController* controller,
                    const google::protobuf::Message* request,
                    google::protobuf::Message* response,
                    google::protobuf::Closure* done);

};
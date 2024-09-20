# 分布式网络通信框架

## 项目简介

本项目是一个基于 C++ 的分布式网络通信框架，使用了 `zookeeper` 进行服务注册与发现，使用 `protobuf` 进行数据序列化与反序列化。通过该框架，用户可以实现高效的 RPC（远程过程调用）服务通信。

## 主要特性

- **服务注册与发现**：基于 `zookeeper` 实现服务动态注册与发现。
- **RPC 支持**：使用 `protobuf` 实现远程过程调用。
- **日志支持**：内置日志系统，支持多种日志级别。
- **跨平台支持**：支持 Linux 系统下的编译和运行。

## 环境依赖

在运行本项目之前，您需要确保以下环境依赖已经安装：

- `C++11` 或更高版本
- `protobuf` 库
- `zookeeper` 库
- `muduo` 网络库
- `cmake` 构建工具

目录结构
├── bin/                     # 可执行文件目录
├── build/                   # 编译生成的文件目录
├── include/                 # 项目公共头文件
├── lib/                     # 外部库依赖
├── src/                     # 源代码
│   ├── mprpcapplication.cc  # RPC应用初始化
│   ├── mprpcconfig.cc       # 配置文件解析
│   ├── rpcprovider.cc       # RPC服务提供者
│   ├── mprpcchannel.cc      # RPC通信通道
│   ├── mprpccontroller.cc   # RPC控制器
│   ├── logger.cc            # 日志模块
│   └── zookeeperutil.cc     # Zookeeper工具类
├── CMakeLists.txt           # CMake配置文件
└── README.md                # 项目说明文件

编译与运行
1. 编译项目 
cd your_project_directory  # 切换到项目根目录
./autobuild.sh  # 执行编译脚本

2. 运行程序

编译成功后，可以在 bin/ 目录下找到可执行文件。运行 provider 服务：


./provider -i test.conf

配置文件

test.conf 是 RPC 服务配置文件的示例，格式如下：

rpcserverip: 127.0.0.1
rpcserverport: 8000
service_name: UserFriendRpc
method_name: GetFriendsList


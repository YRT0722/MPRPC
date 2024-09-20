# aux_source_directory(. SRC_LIST)#当前目录所有源文件添加到SRC变量中
# #set(SRC_LIST mprpcapplition.cc mprpcconfig.cc rpcheader.pb.cc rpcprovider.cc)
# add_library(mprpc  ${SRC_LIST})#//生成一个动态库,静态库不加
# set(SRC_LIST 
#     mprpcapplication.cc 
#     mprpcconfig.cc 
#     rpcheader.pb.cc 
#     rpcprovider.cc 
#     mprpcchannel.cc
#     mprpccontroller.cc
#     logger.cc
#     zookeeperutil.cc
#     )
# #     # zookeeperutil.cc)
# target_link_libraries(mprpc  muduo_net muduo_base pthread zookeeper_mt)
set(SRC_LIST 
    mprpcapplication.cc 
    mprpcconfig.cc 
    rpcheader.pb.cc 
    rpcprovider.cc 
    mprpcchannel.cc
    mprpccontroller.cc
    logger.cc
    zookeeperutil.cc)
    
add_library(mprpc ${SRC_LIST})

target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt )
# 添加额外的搜索路径


# //set(SRC_LIST userservice.cc ../user.pb.cc)
set(SRC_LIST friendservice.cc ../friend.pb.cc)
add_executable(provider ${SRC_LIST})
# 设置库的查找路径 (添加libzookeeper_mt.so.2的路径)

target_link_libraries(provider mprpc protobuf )
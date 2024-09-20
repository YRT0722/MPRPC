#cmake最低版本
cmake_minimum_required(VERSION 3.0)
#项目名字
project(mprpc)

# # 配置编译选项
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

#生成debug，进行gdb调试
set(CMAKE_BUILD_TYPE "Debug")

# 配置头文件搜索路径（如果有）-I
include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/example)
# 配置库文件搜索路径（如果有）-L
link_directories(/${PROJECT_SOURCE_DIR}/lib)

# 设置源文件列表
# set(SRC_LIST main.cpp)

# 设置可执行文件的最终输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#项目设置生成库文件
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
link_directories(/usr/local/lib /home/yrt/下载/分布式网络通信/C++实现分布式网络通信框架项目资料/zookeeper-3.4.10/src/c/.libs)

# 生成可执行文件
# add_executable(server ${SRC_LIST})

# #进入子目录处理目录中cmake文件
add_subdirectory(src)
add_subdirectory(example)

# 链接库文件
# target_link_libraries(server muduo_net muduo_base pthread)

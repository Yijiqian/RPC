set(SRC_LIST calluserservice.cc ../user.pb.cc)
add_executable(consumer ${SRC_LIST})

# 需要用到框架的代码，所有需要连接框架的动态库
target_link_libraries(consumer mprpc protobuf)
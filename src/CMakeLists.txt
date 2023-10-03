# aux_source_directory(. SRC_LIST)
set(SRC_LIST 
    mprpcapplication.cc 
    mprpcconfig.cc 
    rpcprovider.cc  
    rpcheader.pb.cc 
    mprpcchannel.cc 
    mprpccontroller.cc
    logger.cc
    zookeeperutil.cc)
add_library(mprpc ${SRC_LIST})

# muduo_net muduo_base两个静态链接库的顺序一定不能改变， 因为muduo_base依赖于muduo_net
target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt)  
#include <iostream>
#include "mprpcapplication.h"
#include "user.pb.h"


int main(int argc, char **argv)
{
    // 整个程序启动以后，想使用mprpc框架来享受rpc服务调用，一定需要先调用框架的初始化函数(只初始化一次就行了)
    MprpcApplication::Init(argc, argv);

    // 演示调用远程发布的rpc方法Login
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    // rpc方法的请求参数
    /*
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    */
    fixbug::RegisterRequest request;
    request.set_id(308);
    request.set_name("Yijiqian");
    request.set_pwd("buer2197");
    // rpc方法的响应
    // fixbug::LoginResponse response;
    fixbug::RegisterResponse response;
    // 发起rpc方法调用  同步的rpc调用过程  MprpcChannel::callmethod
    // stub.Login(nullptr, &request, &response, nullptr);

    MprpcController controller;
    stub.Register(&controller, &request, &response, nullptr);

    // 一次rpc调用完成，读取调用的结果
    if(controller.Failed())
    {
        std::cout << controller.ErrorText() << std::endl;
    }
    else
    {
        if(response.result().errcode() == 0)
        {
            std::cout << "rpc Register response success: " << response.success() << std::endl;
        }
        else
        {
            std::cout << "rpc Register response error: " << response.result().errmsg() << std::endl;
        }
    }
    


    return 0;
}
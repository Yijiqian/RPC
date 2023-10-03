#include <iostream>
#include <string.h>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include "logger.h"

/*
UserService原来是一个本地服务，提供了两个进程内的本地服务，Login和GetFriendLists
*/
class UserService: public fixbug::UserServiceRpc
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std::cout << "doing local service: Login" << std::endl;
        std::cout << "name:" << name << "pwd:" << pwd << std::endl;
        return false;
    }

    bool Register(uint32_t id, std::string name, std::string pwd)
    {
        std::cout << "doing local service: Register" << std::endl;
        std::cout << "id: " << id << "name: " << name << "pwd: "<< pwd << std::endl;
        return true;
    }

    /*
    重写基类UserServiceRpc的虚函数 下面这些方法都是框架直接调用
    1. caller  ====> Login(LoginRequest)   ====> muduo    ====> callee
    2. callee  ====> Login(LoginRequest)   ====> 交到下面重写的这个Login方法上了
    */ 
    void Login(::google::protobuf::RpcController* controller,
                const ::fixbug::LoginRequest* request,
                ::fixbug::LoginResponse* response,
                ::google::protobuf::Closure* done)
    {
        // 框架给业务上报请求参数LoginRequest，业务获取相应数据做本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool login_result = Login(name, pwd);   // 做本地业务

        // 把响应写入
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(1);
        code->set_errmsg("Login do error!");
        response->set_success(login_result);

        // 执行回调操作    执行响应对象数据的序列化和网络发送（都是由框架完成）
        done->Run();
    }

    void Register(::google::protobuf::RpcController* controller,
                    const ::fixbug::RegisterRequest* request,
                    ::fixbug::RegisterResponse* response,
                    ::google::protobuf::Closure* done)
    {
        // 框架给业务上报请求参数，业务获取相应数据做本地任务
        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool register_result = Register(id, name, pwd);  // 做本地任务

        // 把响应写入
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(register_result);

        // 执行回调函数  执行响应对象数据序列化和网络发送（都是由框架完成）           
        done->Run();

    }

};


int main(int argc, char**argv)
{
    // LOG_INFO("first log message!");
    // LOG_ERR("%s:%s:%d", __FILE__, __FUNCTION__, __LINE__);

    /*
    调用框架的初始化操作，即根据参数argv选择配置文件，并加载配置文件中的信息。
    配置文件中应包含zookeeper的ip地址和端口号。
    */
    MprpcApplication::Init(argc, argv);

    // Provider是一个rpc网络服务对象，把UserService对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService());

    // 启动一个rpc服务发布节点  Run以后，进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();

    return 0;
}
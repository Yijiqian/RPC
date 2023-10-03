#pragma once
#include "lockqueue.h"
#include <string>

enum LogLevel
{
    INFO, // 普通信息
    ERROR,  // 错误信息
};

// Mprpc提供的日志系统
class Logger
{
public:
    // 获取日志的单例
    static Logger& GetInstance();


    // 设置日志级别
    void SetLogLevel(LogLevel level);
    // 写日志
    void Log(std::string msg);
private:
    int m_loglevel;
    LockQueue<std::string> m_lckQue;  // 日志缓冲队列

    Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;

};


/*
snprintf用于格式化输出字符串，并将结果写入到指定的缓冲区。
##__VA_ARGS__：可变参数
*/
// 定义宏  LOG_INFO()
#define LOG_INFO(logmsgformat, ...) \
    do \
    { \ 
        Logger &logger = Logger::GetInstance(); \
        logger.SetLogLevel(INFO); \ 
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__);  \
        logger.Log(c); \
    } while (0);
    

#define LOG_ERR(logmsgformat, ...) \
    do \
    { \ 
        Logger &logger = Logger::GetInstance(); \
        logger.SetLogLevel(ERROR); \ 
        char c[1024] = {0}; \
        snprintf(c, 1024, logmsgformat, ##__VA_ARGS__);  \
        logger.Log(c); \
    } while (0);
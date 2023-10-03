#include "logger.h"
#include <time.h>
#include <iostream>

// 获取日志的单例
Logger& Logger::GetInstance()
{
    static Logger logger;
    return logger;
}

Logger::Logger()
{
    // 启动专门的写日志线程
    std::thread writeLogTask([&](){
        for(;;)
        {
            // 获取当天日期，然后取日志信息，写入相应的日志文件当中
            time_t now = time(nullptr);
            tm *nowtm = localtime(&now);

            char file_name[128];
            sprintf(file_name, "%d-%d-%d-log.txt", nowtm->tm_year+1900, nowtm->tm_mon+1, nowtm->tm_mday);

            /*
            'a': 以“追加”方式打开文件。如果文件不存在，那么创建一个新文件；
                 如果文件存在，那么将写入的数据追加到文件的末尾（文件原有的内容保留）
            "r+": 以“读写”方式打开文件。既可以读取也可以写入，也就是随意更新文件。文件必须存在，否则打开失败。
            "a+": 以“追加/更新”方式打开文件，相当于a和r+叠加的效果。既可以读取也可以写入，也就是随意更新文件。
                  如果文件不存在，那么创建一个新文件；如果文件存在，那么将写入的数据追加到文件的末尾（文件原有的内容保留）。
            */
            FILE *pf = fopen(file_name, "a+");
            if(pf == nullptr)
            {
                std::cout << "logger file : " << file_name << " open error! " << std::endl;
                exit(EXIT_FAILURE);
            }
            std::string msg = m_lckQue.Pop();  // 从队列缓冲区中取数据，由于队列是线程的共享资源，所以需维护一个mutex

            char time_buf[128] = {0};
            sprintf(time_buf, "%d:%d:%d => [%s]", 
                    nowtm->tm_hour, 
                    nowtm->tm_min, 
                    nowtm->tm_sec, 
                    (m_loglevel == INFO ? "info": "error"));
            msg.insert(0, time_buf);
            msg.append("\n");

            fputs(msg.c_str(), pf);
            fclose(pf);
        }
    });

    // 设置分离线程，守护线程
    writeLogTask.detach();
}
    
// 设置日志级别
void Logger::SetLogLevel(LogLevel level)
{
    m_loglevel = level;
}


// 写日志, 把日志信息写入lockqueue缓冲区中
void Logger::Log(std::string msg)
{
    m_lckQue.Push(msg);
}
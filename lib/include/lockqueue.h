#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>


// 异步写日志的日志队列
template<typename T>
class LockQueue
{
public:
    // 多个worker线程都会写日志queue
    void Push(const T &data)
    {
        /*
        std::lock_gurad 是 C++11 中定义的模板类.
        在 lock_guard 对象构造时，传入的 Mutex 对象会被当前线程锁住。
        在lock_guard 对象被析构时，它所管理的 Mutex 对象会自动解锁.
        */
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(data);
        m_condvariable.notify_one();  // notify_one()唤醒任意一个等待条件m_condvariable的线程。
    }

    // 一个线程读日志queue，写日志文件
    T Pop()
    {
        /*
        unique_lock是一个类模板，比lock_guard更加灵活，效率上低一点，内存占用大一点。
        首先是unique_lock可以直接替换lock_guard，调用unique_lock也不需要手动解锁。
        */
        std::unique_lock<std::mutex> lock(m_mutex);
        while(m_queue.empty())
        {
            // 日志队列为空，线程进入wait状态
        }

        T data = m_queue.front();
        m_queue.pop();
        return data;
    }
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condvariable; 
}; 
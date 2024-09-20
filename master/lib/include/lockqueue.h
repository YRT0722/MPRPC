#pragma once

#include<queue>
#include<thread>
#include<mutex>
#include<condition_variable>

//模板类对象
template <class T>
class LockQueue

{
private:
    std::queue<T>m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cond;
 
    /* data */
public:
//模板文件只能写在。h文件中
//多个线程写入日志
    void Push(const T &date)
    {
        std::lock_guard<std::mutex>lock(m_mutex);
        m_queue.push(date);
        m_cond.notify_one();
    }
    T Pop()
    {
        std::unique_lock<std::mutex>lock(m_mutex);
        while (m_queue.empty())
        {
            m_cond.wait(lock);
        }
        T data=m_queue.front();
        m_queue.pop();
        return data;
    }
    
};

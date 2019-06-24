#include <iostream>
#include "Threadpool.h"
#include <unistd.h>
namespace hk
{

void Threadpool::start()
{
    for(size_t idx=0;idx!=_threadNum;++idx)
    {
        unique_ptr<Thread> up(new Thread(
             bind(&Threadpool::threadFunc,this)));
        _threads.push_back(std::move(up));
    }

    for(auto & thread:_threads)
    {
        thread->start();
    }
}

void Threadpool::addTask(Task && task)
{
    _taskque.push(task);
}

void Threadpool::stop()
{
    if(!_isExit) //线程退出标志
    {
        //任务队列还有任务没执行完。等待
        while(!_taskque.empty())
        {
            ::sleep(1);
        }
        //任务队列为空了 退出标志置为true了
        _isExit = true;
        
        _taskque.wakeup();//唤醒所有子线程

        for(auto & thread :_threads)
        {
            thread->join();
        }
    }
}

Task  Threadpool::getTask()
{
    return _taskque.pop();
}


void Threadpool::threadFunc()
{
    while(!_isExit)
    {
        Task  task=getTask();
        if(task)
        {
            task();
        }
    }
}

}//end of namespace hk



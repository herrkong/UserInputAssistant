#pragma once

#include "TaskQueue.h"
#include "Thread.h"
#include <vector>
#include <memory>
#include <functional>
using namespace std;

namespace  hk
{

using Task= function<void()>;

class Threadpool
{
    friend class WorkerThread;
public:
    Threadpool(size_t threadNum,size_t queSize)
    :_threadNum(threadNum)
     ,_queSize(queSize)
     ,_taskque(queSize)
     ,_isExit(false)
    {
       // cout<<"Threadpool()"<<endl;
        _threads.reserve(_threadNum);
    }
    
    void start();
    void stop();
    void addTask(Task && task);//task改成右值引用

private:
    void threadFunc();
    Task  getTask();

private:
    size_t _threadNum;
    size_t _queSize;
    vector<unique_ptr<Thread>> _threads;
    TaskQueue _taskque;
    bool _isExit;
};

}//end of namespace hk

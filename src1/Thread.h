#pragma once
#include "Noncopyable.h"
#include <pthread.h>
#include <functional>
#include <iostream>
using std::endl;
using std::cout;

namespace  hk 
{

class Thread:public Noncopyable
{
public:
    using ThreadCallback=std::function<void()>;
    Thread(ThreadCallback && cb) //移动语义
    :_pthid(0)
     ,_isRunning(false)
     ,_cb(std::move(cb))
    {
        cout<<"Thread()"<<endl;
    }
    ~Thread();

    void start();
    void join();

private:
    static void * threadFunc(void * );

private:
    pthread_t _pthid;
    bool _isRunning;
    ThreadCallback _cb;

};

}

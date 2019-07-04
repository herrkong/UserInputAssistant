//基于对象 不使用继承加多态
#pragma once
#include "Thread.h"
#include <iostream>
using std::endl;
using std::cout;

namespace hk
{
__thread int threadNum = 0 ;

using ThreadCallback=std::function<void()>;

struct ThreadData
{
    int _Num ;
    ThreadCallback _cb ;

    ThreadData(const int Num,ThreadCallback cb)
    :_Num(Num)
    ,_cb(cb)
    {} 

    void runInThread()
    {
        //任务执行之前:do something
        threadNum = _Num ;
        cout<<"我是"<<threadNum<<" 号线程"<<endl;
        if(_cb)
        {
            _cb();
        }
        //任务执行之后:do something
    }
};

Thread::~Thread()
{
   // cout<<"~Thread()"<<endl;
    if(_isRunning)
    {
        pthread_detach(_pthid);
        _isRunning=false;
    }
}
 
void Thread::start()
{
    ThreadData * data = new ThreadData(_Num,_cb);
    if(pthread_create(&_pthid,NULL,threadFunc,data))
    {
        perror("pthread_create");
        return;
    }
    _isRunning = true;
}

void Thread::join()
{
    if(_isRunning)
    {
        pthread_join(_pthid,NULL);
        _isRunning =false;
    }
}


void * Thread::threadFunc(void * arg)
{  
    
    ThreadData * pdata = static_cast<ThreadData *>(arg);
    if(pdata)
    {
        pdata->runInThread();
    }
    delete pdata;
    return nullptr;
}

}//namespace hk

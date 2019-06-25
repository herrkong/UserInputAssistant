//基于对象 不使用继承加多态


#include "Thread.h"
#include <iostream>
using std::endl;
using std::cout;



namespace hk
{

Thread::~Thread()
{
    cout<<"~Thread()"<<endl;
    if(_isRunning)
    {
        pthread_detach(_pthid);
        _isRunning=false;
    }
}

void Thread::start()
{

    if(pthread_create(&_pthid,NULL,threadFunc,this))
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
    
    Thread * pthread =static_cast<Thread *>(arg);
    if(pthread)
    {
        pthread->_cb();
    }
    return nullptr;
}

}//namespace hk

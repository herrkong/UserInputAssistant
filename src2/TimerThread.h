#pragma once
#include "Timer.h"
#include "Thread.h"

namespace  hk
{


class TimerThread
{
    using TimerCallback =function<void()>;
public:
    TimerThread(int initialTime,int intervalTime,TimerCallback && cb)
    :_timer(initialTime,intervalTime,std::move(cb))
     ,_thread(bind(&Timer::start,&_timer))
    {
        cout<<"TimerThread(int initialTime,int intervalTime,TimerCallback && cb)"<<endl;
    }
    ~TimerThread()
    {
        cout<<"~TimerThread()"<<endl;
    }

    void start()
    {
        _thread.start();
    }

    void stop()
    {
        _timer.stop();
        _thread.join();
    }




private:
    Timer _timer;
    Thread _thread;
    
};

}//end of namespace hk

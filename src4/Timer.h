#pragma once
#include <functional>
#include <iostream>
#include <poll.h>//nb 用的poll
#include <sys/timerfd.h>
#include <unistd.h>
#include <iostream>
using namespace std;

namespace  hk
{

class Timer
{
    using TimerCallback = function<void()>;
public:
    Timer(int initTime,int intervalTime,TimerCallback && cb)
    :_fd(createTimerfd())
     ,_initialTime(initTime)
     ,_intervalTime(intervalTime)
     ,_cb(move(cb))
     ,_isStarted(false)
    {
        cout<<"Timer(int initTime,int intervalTime,TimerCallback && cb)"<<endl;
    }
    ~Timer()
    {
        cout<<"~Timer()"<<endl;
    }

    void start();
    void stop();
    void setTimer(int initTime,int intervalTime);
    void handleRead();
private:
    int createTimerfd();


private:
    int _fd;
    int _initialTime;
    int _intervalTime;
    TimerCallback _cb;
    bool _isStarted;

};

}//end of namespace hk

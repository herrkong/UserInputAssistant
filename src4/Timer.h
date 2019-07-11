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
    static Timer * createTimer();
    static void destroy();
    Timer * initTime(int initTime,int intervalTime,
                     TimerCallback && cb);

    void start();
    void stop();
    int  getFd();
    void setTimer(int initTime,int intervalTime);
    void handleRead();//包含读描述符和执行回写事件

private:
    Timer(){}
    ~Timer(){}
    int createTimerfd();


private:
    static Timer * _ptimer ;
    int _fd;
    int _initialTime;
    int _intervalTime;
    TimerCallback _cb;
    bool _isStarted;

};

}//end of namespace hk

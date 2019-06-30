#include "Timer.h"

using namespace std;

namespace hk
{

void Timer::start()
{
    _isStarted = true;
    setTimer(_initialTime,_intervalTime);

    struct pollfd pfd;
    pfd.fd = _fd;
    pfd.events = POLLIN ;

    while(_isStarted)
    {
        int nready = ::poll(&pfd,1,5000);
        if(-1 == nready && errno == EINTR)
            continue;
        else if(-1 == nready)
        {
            perror("poll");
            return ;
        }
        else if(0 == nready)
        {
            cout<<"poll timeout "<<endl;
        }
        else 
        {
            if(pfd.revents & POLLIN)
            {
                handleRead(); //处理读事件
                //如果没有处理该事件，会一直触发
                if(_cb)
                    _cb();//执行任务
            }
        }
    }//while
}


void Timer::stop()
{
    if(_isStarted)
    {
        setTimer(0,0);
        _isStarted = false ;
    }
}

void Timer::setTimer(int initialTime,int intervalTime)
{
    struct itimerspec value ;
    value.it_value.tv_sec = initialTime ;
    value.it_value.tv_nsec = 0;
    value.it_interval.tv_sec = intervalTime ;
    value.it_interval.tv_nsec = 0;

    int ret = ::timerfd_settime(_fd,0,&value,nullptr);
    if(-1 == ret)
    {
        perror("timerfd_settime");
    }
}

int Timer::createTimerfd()
{
    int fd = ::timerfd_create(CLOCK_REALTIME,0);
    if(-1 == fd)
    {
        perror("timerfd_create");
    }
    return fd;
}

void Timer::handleRead()
{
    //Timerfd对应的缓冲区大小为8个字节
    uint64_t howmany;//超时次数
    int ret = ::read(_fd,&howmany,sizeof(howmany));
    if(ret != sizeof(howmany))//如果ret不等于8
    {
        perror("read");
    }
}



}//end of namespace hk

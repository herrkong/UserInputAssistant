#pragma once
#include "MutexLock.h"
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <functional>
#include <sys/epoll.h>

using namespace std;

namespace  hk
{

class Acceptor;
class TcpConnection;


class EventLoop
{
public:
    using TcpConnectionPtr = shared_ptr<TcpConnection>;
    using TcpConnectionCallback =
        std::function<void(const TcpConnectionPtr &)>;
    //绑定了const TcpConnectionPtr & 参数的函数对象
    using Functor = std::function<void()>;
    EventLoop(Acceptor & acceptor);
    void loop();
    void unloop();
    void runInLoop(Functor && cb);    
    //视频14:58   version2 
    //右值引用,移动语义
    void setConnectionCallback(TcpConnectionCallback && cb)
    { _onConnection = std::move(cb); }
    void setMessageCallback(TcpConnectionCallback && cb)
    { _onMassage = std::move(cb); }
    void setCloseCallback(TcpConnectionCallback && cb)
    { _onClose = std::move(cb); }

private:
    void waitEpollFd();
    void handleNewConnection();
    void handleMessage(int fd);
    void handleRead();
    void wakeup();
    int createEpollFd();
    int createEventFd();
    void doPendingFunctors();

    void addEpollFdRead(int fd);
    void delEpollFdRead(int fd);
    bool isConnectionClosed(int fd);

private:
    int _efd;
    int _eventfd;
    Acceptor & _acceptor;
    vector<struct epoll_event> _eventList;
   // map<int,TcpConnection> _conns;
    map<int,TcpConnectionPtr> _conns;
    bool _isLooping;
    
    MutexLock _mutex;
    vector<Functor> _pendingFunctors;

    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMassage;
    TcpConnectionCallback _onClose;

};

}//end of namespace hk

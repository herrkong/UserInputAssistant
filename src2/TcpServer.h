#pragma once
#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include <memory>
#include <functional>

namespace hk
{

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback = 
    std::function<void(const TcpConnectionPtr &)>;


class TcpServer
{
public:
    TcpServer(const string & ip,unsigned short port);
    void start();

    //为什么都是右值引用
    void setConnectionCallback(TcpConnectionCallback && cb);
    void setMessageCallback(TcpConnectionCallback  && cb);
    void setCloseCallback(TcpConnectionCallback && cb);

private:
    Acceptor _acceptor;
    EventLoop _loop;

};


}//end of namespace hk

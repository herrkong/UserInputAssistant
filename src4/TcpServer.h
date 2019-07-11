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
    void start();  //开始服务
    void stop();   //停止服务

    //设置回调函数
    void setConnectionCallback(TcpConnectionCallback && cb);
    void setMessageCallback(TcpConnectionCallback  && cb);
    void setCloseCallback(TcpConnectionCallback && cb);

private:
    Acceptor _acceptor;//服务端接受器
    EventLoop _loop;

};


}//end of namespace hk

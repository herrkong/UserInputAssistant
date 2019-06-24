#pragma once
#include "InetAddress.h"
#include "Socket.h"
#include "Noncopyable.h"
#include "SocketIO.h"
#include <functional>
#include <memory>
using namespace std ;

namespace  hk
{
//version 2新增
class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;

//enable_shared_from_this 保证多个share_ptr指向同一个对象

class EventLoop;

class TcpConnection
:Noncopyable
,public std::enable_shared_from_this<TcpConnection> //第四处
//类继承的时候 默认是私有的 这里需要public继承 因为需要用它的方法
//而Nocopyable只继承它的特性。所以是私有的。
{
public:
    TcpConnection(int fd,EventLoop * loop);
    ~TcpConnection();
    string receive();
    void send(const string & msg);
    void sendInLoop(const string & msg);//新增

    string toString() const ;
    void shutdown();
        
    void setConnectionCallback(const TcpConnectionCallback & cb);
    void setMessageCallback(const TcpConnectionCallback & cb);
    void setCloseCallback(const TcpConnectionCallback & cb);

    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();

private:
    InetAddress getLocalAddr();
    InetAddress getPeerAddr();
private:
    Socket _sock;
    SocketIO _socketIo;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    bool _isShutdownWrite;
    EventLoop * _loop;

    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;

};

}//end of namespace hk

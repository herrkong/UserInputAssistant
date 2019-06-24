#include "TcpConnection.h"
#include "InetAddress.h"
#include "EventLoop.h"
#include <sstream>

using namespace std;

namespace hk
{

TcpConnection::TcpConnection(int fd,EventLoop * loop)
:_sock(fd)
,_socketIo(fd)
,_localAddr(getLocalAddr())
,_peerAddr(getPeerAddr())
,_isShutdownWrite(false)
,_loop(loop)
{
    cout<<"TcpConnection(int fd,EventLoop * loop)"<<endl;
}


TcpConnection::~TcpConnection()
{
    if(_isShutdownWrite)
        shutdown();
    cout<<"~TcpConnection()"<<endl;
}

void TcpConnection::send(const string & msg)
{
    _socketIo.writen(msg.c_str(),msg.size());

}

void TcpConnection::sendInLoop(const string & msg)
{
    _loop->runInLoop(std::bind(&TcpConnection::send,this,msg));
}


void TcpConnection::shutdown()
{
    if(_isShutdownWrite)
    {
        _isShutdownWrite = true;
        _sock.shutdownwrite();
    }
}

string TcpConnection::receive()
{
    char buf[65536] = {0};
    _socketIo.readline(buf,sizeof(buf));
    return string(buf);
}

string TcpConnection::toString() const
{
    ::ostringstream oss;
    oss<<_localAddr.ip()<< " : " <<_localAddr.port()
        <<" --> "<<_peerAddr.ip()<<" : "<<_peerAddr.port()<<endl;
    return oss.str();

}


InetAddress TcpConnection::getLocalAddr()
{
    struct sockaddr_in addr ;
    socklen_t len =sizeof(struct sockaddr);
    if(-1==getsockname(_sock.fd(),(struct sockaddr *)& addr,&len))
        perror("getsockname");
    return InetAddress(addr);
}


InetAddress TcpConnection::getPeerAddr()
{
    struct sockaddr_in addr ;
    socklen_t len = sizeof (struct sockaddr);
    if(-1==getpeername(_sock.fd(),(struct sockaddr *)& addr,&len))
        perror("getpeername");//第二处 
    return InetAddress(addr);
}


void TcpConnection::setConnectionCallback(const TcpConnectionCallback & cb)
{
    _onConnection = std::move(cb);
    //_onConnection = cb;
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback & cb)
{
    _onMessage = std::move(cb);
   // _onMessage = cb;
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback & cb)
{
    _onClose = std::move(cb);
    //_onClose = cb;
}

void TcpConnection::handleConnectionCallback()
{
    if(_onConnection)
    {   //在本对象内部使用的指针
        _onConnection(shared_from_this());
    }    
}

void TcpConnection::handleMessageCallback()
{
    if(_onMessage)
    {
        _onMessage(shared_from_this());
    }

}

void TcpConnection::handleCloseCallback()
{
    if(_onClose)
    {
        _onClose(shared_from_this());
    }
}

}//end of namespace hk

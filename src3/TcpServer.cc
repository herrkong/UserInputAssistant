#include "TcpServer.h"

namespace  hk
{

TcpServer::TcpServer(const string & ip,unsigned short port)
:_acceptor(ip,port)
,_loop(_acceptor)
{
    cout<<"TcpServer(const string & ip,unsigned short port)"<<endl;
}


void TcpServer::start()
{
    _acceptor.ready();
    _loop.loop();
}


void TcpServer::setConnectionCallback(TcpConnectionCallback && cb)
{
    _loop.setConnectionCallback(std::move(cb));
}

void TcpServer::setMessageCallback(TcpConnectionCallback && cb)
{
    _loop.setMessageCallback(std::move(cb));
}

void TcpServer::setCloseCallback(TcpConnectionCallback && cb)
{
    _loop.setCloseCallback(std::move(cb));
}


}//end of namespace hk

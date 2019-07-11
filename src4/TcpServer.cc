#include "TcpServer.h"

namespace  hk
{

TcpServer::TcpServer(const string & ip,unsigned short port)
:_acceptor(ip,port)
,_loop(_acceptor)
{
   // cout<<"TcpServer(const string & ip,unsigned short port)"<<endl;
}


void TcpServer::start()
{
    //cout<<"111"<<endl;
    _acceptor.ready();
    //cout<<"222"<<endl;
   //问题出在loop这里 EventLoop 
    _loop.loop();
    //cout<<"333"<<endl;
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

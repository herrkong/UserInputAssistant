#pragma once
#include "InetAddress.h"
#include "Socket.h"

namespace  hk
{


class Acceptor
{
public:
    Acceptor(unsigned short port);
    Acceptor(const string & ip,unsigned short port);
    ~Acceptor();
    int accept();
    void ready();
    int fd() const { return _listensock.fd();}//version2中新增

private:
    void setReuseAddr(bool on);
    void setReuseport(bool on);
    void bind();
    void listen();

private:
    InetAddress _addr;
    Socket _listensock;

};


}//end of namespace hk

#include "Acceptor.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
using  namespace std;

namespace hk 
{

Acceptor::Acceptor(unsigned short port)
:_addr(port)
,_listensock()
{
    cout<<"Acceptor(port)"<<endl;
}


Acceptor::Acceptor(const string & ip,unsigned short port)
:_addr(ip,port)
,_listensock()
{
    
    cout<<"Acceptor(string & ip,unsigned short port)"<<endl;
}


Acceptor::~Acceptor()
{
    cout<<"~Acceptor()"<<endl;
}



void Acceptor::setReuseAddr(bool on)
{

    int one = on;
    if(setsockopt(_listensock.fd()
                  ,SOL_SOCKET
                  ,SO_REUSEADDR
                  ,&one
                  ,sizeof(one)) < 0)
    {
        perror("setsockopt");
    }
}


void Acceptor::setReuseport(bool on)
{
    int one = on;
    if(setsockopt(_listensock.fd()
                  ,SOL_SOCKET
                  ,SO_REUSEPORT  //一处
                  ,&one
                  ,sizeof(one))<0)
    {
        perror("setsockopt");
    }
}    

void Acceptor::ready()
{
    setReuseport(true);
    setReuseAddr(true);
    bind();
    listen();
}

void Acceptor::bind()
{
    int ret =::bind(_listensock.fd(),
                  (struct sockaddr *)_addr.getInetAddressPtr(),
                  sizeof(struct sockaddr));
    if(-1==ret)
    {
        perror("bind");
    }
}

void Acceptor::listen()
{
    int ret = ::listen(_listensock.fd(),10);
    if(-1==ret)
    {
        perror("listen");
    }
}



int Acceptor::accept()
{
    int peerfd = ::accept(_listensock.fd(),NULL,NULL);
    if(-1 == peerfd)
    {
        perror("accept");
    }
    return peerfd;
}



}//end of namespace hk

#pragma once
#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include <arpa/inet.h>
using namespace std;

namespace  hk
{

class InetAddress
{
public:
    InetAddress(unsigned  short port); //本机只需要传端口号 
    InetAddress(const string & ip,unsigned short port);
    InetAddress(const struct sockaddr_in & addr);
    
    unsigned short port() const;
    string  ip() const;
    struct sockaddr_in * getInetAddressPtr() {return &_addr;}
      

private:
    struct sockaddr_in _addr;
};

}//end of namespace hk

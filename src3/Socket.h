#pragma once
#include <iostream>
using namespace std;

namespace hk
{

class Socket
{
public:
    Socket(int fd);
    explicit//抑制隐式转换
    Socket();
    ~Socket();
    int fd() const; //返回sockfd
    void shutdownWrite();//关闭套接字的写端口

 
private:
    int _fd;

};

}//end of namespace hk

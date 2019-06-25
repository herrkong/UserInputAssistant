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
    int fd() const;
    void shutdownWrite();

 
private:
    int _fd;

};

}//end of namespace hk

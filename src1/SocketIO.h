#pragma once
#include "Socket.h"

namespace hk
{

class SocketIO
{
public:
    SocketIO(int fd);
    ~SocketIO();

    int readn(char * buf,int len);
    int writen(const char * buf,int len);
    int readline(char * buf,int maxlen);
   // int readInt32(int);
    //int writeInt32(int);
    int recvPeek(char * buf,int len );
    //这里是干啥的。。。
private:
    int _fd;

};

}//end of namespace hk

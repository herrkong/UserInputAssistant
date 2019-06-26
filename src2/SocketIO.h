#pragma once
#include "Socket.h"

namespace hk
{

typedef struct
{
    int dataLen;//不管这个数是多大 它就是四个字节啊。我擦。
    char buf[1024];
}Train_t;





class SocketIO
{
public:
    explicit SocketIO(int fd);
    ~SocketIO();
    
    int sendTrain(const char * buf);
    int recvTrain(char * buf);

   // int readn(char * buf,int len);
   // int writen(const char * buf,int len);
   // int readline(char * buf,int maxlen);
   // int readInt32(int);
    //int writeInt32(int);
   // int recvPeek(char * buf,int len );
    //这里是干啥的。。。
private:
    int _fd;

};

}//end of namespace hk


#include "Socket.h"

#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>

namespace  hk
{


Socket::Socket(int fd)                                                  
:_fd(fd)
{
    cout<<"Socket(int fd)"<<endl;

}

Socket::Socket() 
{
    _fd=socket(AF_INET,SOCK_STREAM,0);
    
    if(-1==_fd)
    {
        perror("socket()");
    }

    cout<<"Socket()"<<endl;

}

int Socket::fd() const
{
    return _fd;
}

Socket::~Socket()
{   
    ::close(_fd);
    cout<<"~Socket()"<<endl;
}

void Socket::shutdownwrite()
{
   ::shutdown(_fd,SHUT_WR);
}


}//end of namespace hk

#include "SocketIO.h"
#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h> //read ,write
using namespace std;
namespace  hk
{

SocketIO::SocketIO(int fd)
    :_fd(fd)
{
    cout<<"SocketIO(int fd)"<<endl;
}

SocketIO::~SocketIO()
{
    cout<<"~SocketIO()"<<endl;
}

//返回接受了多少数据
int SocketIO::readn(char * buf,int len)
{
    int left = len;
    char * p = buf;
    while(left>0)
    {
        int ret =read(_fd,p,left); 
        if(-1 == ret && errno == EINTR)//中断信号 下次继续传入
            continue;

        else if(-1 == ret)
        {
            perror("read");
            return len-left;
        }

        else if(0 == ret) //链接断开了。
        {
            return len-left;
        }
        else
        {
            left -= ret;
            p += ret;//指针移动
        }
    }
    return len - left;
}

//返回发送多少数据
int SocketIO::writen(const char * buf,int len)
{
    int left = len ;
    const char * p = buf ;
    while(left>0)
    {
        int ret = write(_fd,p,left);
        if(-1 == ret && errno == EINTR)
            continue;
        else if(-1 == ret)
        {
            perror("write");
            return len - left;
        }
        else 
        {
            left -= ret ;
            p += ret;
        }

    }//while
    return len - left;
}

//读取一行数据
int SocketIO::readline(char * buf,int maxlen)
{
    int left = maxlen -1;
    char * p = buf ;
    int ret ;
    int total = 0 ;
    while (left > 0)
    {
        ret = recvPeek(p,left);
        for(int idx = 0; idx != ret ; ++ idx )
        {
            if('\n' == p[idx])
            {
                int sz = idx + 1 ;
                readn(p,sz);
                total += sz;
                p+=sz;
                *p='\0';
                return total;
            }
        }//for
        //如果没有发现'\n'
        readn(p,ret);
        left -= ret ;
        p += ret ;
        total += ret ;
    }//while
    *p='\0';
    return total ;
}

int SocketIO::recvPeek(char * buf,int len)
{
    int ret;
    do
    {
        ret = ::recv(_fd,buf,len,MSG_PEEK);
    
    }while(-1 == ret && errno == EINTR);
    
    return ret ;
}

}//end of namespace hk

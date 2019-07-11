#pragma once
#include "TcpServer.h"
#include "Threadpool.h"
#include "MyConf.h"
#include "MyDict.h"
#include "MyTask.h"
#include "Timer.h"
namespace  hk
{

class UserInputAssitant
{
public:
    UserInputAssitant(const string & configFilePath);

    //被注册回调函数，提供给TcpServer使用。
    void onConnection(const TcpConnectionPtr & conn);
    void onMessage(const TcpConnectionPtr & conn);
    void onClose(const TcpConnectionPtr & conn);
   
    void start();//开始服务
    void stop();//停止服务


private:
    MyConf  _conf;//配置文件对象
    CacheManager * _pCacheManager ;
    Timer * _pTimer;
    Threadpool _threadpool;//线程池对象
    TcpServer _tcpServer;//网络编程对象
};



}//end of namespace hk

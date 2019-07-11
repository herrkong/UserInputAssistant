
#pragma once
#include "UserInputAssitant.h"

namespace  hk
{

UserInputAssitant::UserInputAssitant(const string & configFilePath)
:_conf(configFilePath)
,_pCacheManager(CacheManager::getCacheManager()->initCache())
//初始化主Cache,并首次同步Cache
,_pTimer(Timer::createTimer()->initTime(
    stoi(_conf.getConfigMap()["begTime"]),
    stoi(_conf.getConfigMap()["valTime"]),
    bind(&CacheManager::periodicUpdateCaches,_pCacheManager)                                           ))
,_threadpool(4,10)
,_tcpServer(_conf.getConfigMap().find("ip")->second,
            stoi(_conf.getConfigMap().find("port")->second))
{
   // MyDict * pMydict = MyDict::getInstance();
   // pMydict->en_init(
        // _conf.getConfigMap()["dict"].c_str(),
        // _conf.getConfigMap()["index"].c_str()
                   // );
    _threadpool.start();
    _pTimer->start();
    cout<<"服务器准备就绪!"<<endl;

}

void UserInputAssitant::onConnection(const TcpConnectionPtr & conn)
{
    cout<<"连接中..."<<endl;
    cout<<conn->toString()<<"连接成功！"<<endl;
    conn->send("服务器配置成功！");
}

void UserInputAssitant::onMessage(const TcpConnectionPtr & conn)
{
    cout<<"数据发送中..."<<endl;
    string msg = conn->receive();
    cout<<" >>收到客户端数据: "<<msg<<endl;
    

    //初始化词典
    MyDict * pDict = MyDict::getInstance();

    pDict->en_init(_conf.getConfigMap().find("dict")->second.c_str(),
                _conf.getConfigMap().find("index")->second.c_str());

    //初始化MyTask对象
    MyTask mytask(*pDict,msg,conn);


    //把任务交给线程池处理
    _threadpool.addTask(std::bind(&MyTask::execute,mytask));

}

void UserInputAssitant::onClose(const TcpConnectionPtr & conn)
{
    cout<<"断开连接中..."<<endl;
    cout<<conn->toString()<<"断开连接成功."<<endl;
}

using namespace placeholders;

void UserInputAssitant::start()
{
    //cout<<"233"<<endl;
    _tcpServer.setConnectionCallback(std::bind(&UserInputAssitant::onConnection, this, _1));
   // cout<<"xxx"<<endl;
    _tcpServer.setMessageCallback(std::bind(&UserInputAssitant::onMessage, this, _1));
   // cout<<"qqq"<<endl;
    _tcpServer.setCloseCallback(std::bind(&UserInputAssitant::onClose, this, _1));
  // cout<<"www"<<endl;
   //""问题出自这里TcpServer
    _tcpServer.start();
   // cout<<"666"<<endl;

}


}//end of namespace hk

#include "UserInputAssitant.h"

namespace  hk
{

UserInputAssitant::UserInputAssitant(const string & configFilePath)
:_conf(configFilePath)
,_tcpServer(_conf.getConfigMap().find("ip")->second,
            stoi(_conf.getConfigMap().find("port")->second))
,_threadpool(4,10)
{
    _threadpool.start();
    cout<<"初始化UserInputAssistant()"<<endl;
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

    pDict->init(_conf.getConfigMap().find("dict")->second.c_str(),
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

    _tcpServer.setConnectionCallback(std::bind(&UserInputAssitant::onConnection, this, _1));
    _tcpServer.setMessageCallback(std::bind(&UserInputAssitant::onMessage, this, _1));
    _tcpServer.setCloseCallback(std::bind(&UserInputAssitant::onClose, this, _1));
    _tcpServer.start();

}


}//end of namespace hk

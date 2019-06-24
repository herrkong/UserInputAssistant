#include "UserInputServer.h"

using namespace hk;

UserInputServer::UserInputServer(const string & configFilePath)
:_conf(configFilePath)
,_tcpServer(_conf.getConfigMap().find("ip")->second,stoi(_conf.getConfigMap().find("port")->second))
,_threadpool(4,10)
{
    _threadpool.start();
}

using namespace placeholders;

void UserInputServer::start()
{
    _tcpServer.setConnectionCallback(std::bind(&UserInputServer::onConnection,this, _1));
    _tcpServer.setMessageCallback(std::bind(&UserInputServer::onMessage,this,_1));
    _tcpServer.setCloseCallback(std::bind(&UserInputServer::onClose,this,_1));
    _tcpServer.start();
}


void UserInputServer::onConnection(const TcpConnectionPtr & conn)
{
    cout<<conn->toString()<<"has connected!"<<endl;
    conn->send("welcome to server.");
}

void UserInputServer::onMessage(const TcpConnectionPtr & conn)
{
    string msg = conn->receive();
    cout<<">> receive msg from client: "<<msg<<endl;
    
    MyDict * pDict = MyDict::getInstance();
    
    pDict->init(_conf.getConfigMap().find("dict")->second.c_str(),
               _conf.getConfigMap().find("index")->second.c_str());
    
    MyTask mytask(*pDict,msg,conn);
    
    _threadpool.addTask(std::bind(&MyTask::query,mytask));
}

void UserInputServer::onClose(const TcpConnectionPtr & conn)
{
    cout<<conn->toString()<<" has closed!"<<endl;
}


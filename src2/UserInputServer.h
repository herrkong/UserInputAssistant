#pragma once
#include "MyConf.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "Threadpool.h"
#include "MyTask.h"
namespace  hk
{

class UserInputServer
{
public:
   explicit  UserInputServer(const string & configFileName);
   void start();
   
   void onConnection(const TcpConnectionPtr & conn);
   void onMessage(const TcpConnectionPtr & conn);
   void onClose(const TcpConnectionPtr & conn);


private:
   MyConf _conf;
   TcpServer _tcpServer;
   Threadpool _threadpool;

};

}//end of namespace hk

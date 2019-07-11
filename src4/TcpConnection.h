 
#ifndef __HK_TCPCONNECTION_H__
#define __HK_TCPCONNECTION_H__

#include "Socket.h"
#include "InetAddress.h"
#include "SocketIO.h"
#include "Noncopyable.h"

#include <string>
#include <memory>
#include <functional>
using std::string;
using std::function;

namespace hk
{


class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
//函数对象 参数是指向TcpConnnection对象的指针 

class EventLoop;

class TcpConnection
: Noncopyable
, public std::enable_shared_from_this<TcpConnection>
{
public:
	TcpConnection(int fd, EventLoop * loop );
	~TcpConnection();

	string receive();
	void send(const string & msg);
	void sendInLoop(const string & msg);

	string toString() const;
	void shutdown();

	void setConnectionCallback(const TcpConnectionCallback & cb);
	void setMessageCallback(const TcpConnectionCallback & cb);
	void setCloseCallback(const TcpConnectionCallback & cb);

	void handleConnectionCallback();
	void handleMessageCallback();
	void handleCloseCallback();
private:
	InetAddress getLocalAddr();
	InetAddress getPeerAddr();
private:
	Socket _sock;
	SocketIO _socketIo;
	InetAddress _localAddr;
	InetAddress _peerAddr;
	bool _isShutdwonWrite;
	EventLoop * _loop;

	TcpConnectionCallback _onConnection;
	TcpConnectionCallback _onMessage;
	TcpConnectionCallback _onClose;

};

}//end of namespace hk

#endif

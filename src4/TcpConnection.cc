
#include "TcpConnection.h"
#include "InetAddress.h"
#include "EventLoop.h"

#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>

#include <sstream>

namespace hk
{
TcpConnection::TcpConnection(int fd, EventLoop * loop)
: _sock(fd)
, _socketIo(fd)
, _localAddr(getLocalAddr())
, _peerAddr(getPeerAddr())
, _isShutdwonWrite(false)
, _loop(loop)
{
}

TcpConnection::~TcpConnection()
{
	if(!_isShutdwonWrite) {
		shutdown();
	}
}

string TcpConnection::receive()
{
	char buf[65536] = {0};
	//_socketIo.readline(buff, sizeof(buff));
	_socketIo.recvTrain(buf);   
    return string(buf);
}
	
void TcpConnection::send(const string & msg)
{
	//_socketIo.writen(msg.c_str(), msg.size());
    _socketIo.sendTrain(msg.c_str());

}

void TcpConnection::sendInLoop(const string & msg)
{
   // cout<<"before sendInLoop()"<<endl;
	_loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
    //cout<<"after sendInLoop()"<<endl;
}

void TcpConnection::shutdown()
{
	if(!_isShutdwonWrite) {
		_isShutdwonWrite = true;
		_sock.shutdownWrite();
	}
}

string TcpConnection::toString() const
{
	std::ostringstream oss;
	oss <<"主机服务器:"<<_localAddr.ip() << ":"<<_localAddr.port() << " --> "
		<<"客户端:"<<_peerAddr.ip() << ":" << _peerAddr.port();
	return oss.str();
}


InetAddress TcpConnection::getLocalAddr()
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr);
	if(getsockname(_sock.fd(), (struct sockaddr*)&addr, &len) == -1) {
		perror("getsockname");
	}
	return InetAddress(addr);
}

InetAddress TcpConnection::getPeerAddr()
{
	struct sockaddr_in addr;
	socklen_t len = sizeof(struct sockaddr);
	if(getpeername(_sock.fd(), (struct sockaddr*)&addr, &len) == -1) {
		perror("getpeername");
	}
	return InetAddress(addr);
}

void TcpConnection::setConnectionCallback(const TcpConnectionCallback & cb)
{
	_onConnection = std::move(cb);
}

void TcpConnection::setMessageCallback(const TcpConnectionCallback & cb)
{
    //这里又打印了。。。
    _onMessage = std::move(cb);
    cout<<"191919"<<endl;
}

void TcpConnection::setCloseCallback(const TcpConnectionCallback & cb)
{
	_onClose = std::move(cb);
}

void TcpConnection::handleConnectionCallback()
{
	if(_onConnection) {
		_onConnection(shared_from_this());
	}
}

void TcpConnection::handleMessageCallback()
{
   // cout<<"171717"<<endl;
    //_onMessage()的问题
	if(_onMessage) {
		_onMessage(shared_from_this());
	}
   // cout<<"181818"<<endl;
}

void TcpConnection::handleCloseCallback()
{
	if(_onClose) {
		_onClose(shared_from_this());
	}
}

}//end of namespace hk

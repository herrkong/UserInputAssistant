#include "EventLoop.h" 
#include "Acceptor.h"
#include "TcpConnection.h"

#include <unistd.h>
#include <assert.h>
#include <sys/eventfd.h>

#include <iostream>
using std::cout;
using std::endl;

namespace hk
{

EventLoop::EventLoop(Acceptor & acceptor)
: _efd(createEpollFd())
, _eventfd(createEventFd())
, _acceptor(acceptor)
, _eventList(1024)
, _isLooping(false)
{
	addEpollFdRead(_acceptor.fd());
	addEpollFdRead(_eventfd);
}

void EventLoop::loop()
{
    //cout<<"111"<<endl;
	_isLooping = true;
	while(_isLooping) {
       // cout<<"222"<<endl;
        //问题在出在了这里 waitEpollFd 
		waitEpollFd();
       // cout<<"333"<<endl;
	}
   // cout<<"444"<<endl;
}

void EventLoop::unloop()
{
	if(_isLooping) 
		_isLooping = false;
}

void EventLoop::runInLoop(Functor && cb)
{
	{
	    MutexLockGuard autolock(_mutex);
	    _pendingFunctors.push_back(std::move(cb));
	}

	wakeup();
}

//问题在这里
void EventLoop::waitEpollFd()
{
	cout<<"111"<<endl;
    int nready;
	do {
		nready = epoll_wait(_efd, &*_eventList.begin(), _eventList.size(), 5000);
        cout<<"222"<<endl;
	}while(nready == -1 && errno == EINTR);
    cout<<"333"<<endl;
	
    if(nready == -1) 
    {
		perror("epoll_wait");
		return;
	} 
    else if(nready == 0) 
    {
		cout << ">> epoll_wait timeout!" << endl;
	} 
    else 
    {
		if(size_t(nready) == _eventList.size()) 
        {
            //就绪事件列表扩容
			_eventList.resize(2 * nready);
		}

		for(int idx = 0; idx != nready; ++idx) 
        {
            cout<<"444"<<endl;
			int fd = _eventList[idx].data.fd;
			
            if(fd == _acceptor.fd()) 
            {
				cout<<"555"<<endl;
                //处理新连接
				if(_eventList[idx].events & EPOLLIN) 
                {
					handleNewConnection();
                    cout<<"666"<<endl;
				}
			}
            else if(fd == _eventfd) //处理事件
            {
				if(_eventList[idx].events & EPOLLIN)
                {
                    cout<<"777"<<endl;
                    //handleRead 有点问题
					handleRead();
				    cout << ">>before doPendingFunctors()" << endl;
					doPendingFunctors();//在这里发送数据
					cout << ">>after doPendingFunctors()" << endl;
				}
			}
        #if 0
            else if(fd == _timerfd) //处理时间超时回写事件
            {
                if(_eventList[idx].events & EPOLLIN)
                {
                    cout<<"更新缓存"<<endl;
                    _pTimer->Timer::handleRead();
                    //包含读取描述符和回写事件
                }
            }
        #endif   
            else 
            {
				 //处理消息
				if(_eventList[idx].events & EPOLLIN) 
                {
                    cout<<"888"<<endl;
                    //这里也有点问题
					handleMessage(fd);
                    cout<<"999"<<endl;
				}
			}
		}//for
	}//else
}

void EventLoop::handleNewConnection()
{
	int peerfd = _acceptor.accept();
	addEpollFdRead(peerfd);
	TcpConnectionPtr conn(new TcpConnection(peerfd, this));
	conn->setConnectionCallback(_onConnection);
	conn->setMessageCallback(_onMessage);
	conn->setCloseCallback(_onClose);

	_conns.insert(std::make_pair(peerfd, conn));

	conn->handleConnectionCallback();
}

//这里看看
void EventLoop::handleMessage(int fd)
{
   // cout<<"101010"<<endl;
	bool isClosed = isConnectionClosed(fd);
   // cout<<"111111"<<endl;
	auto iter = _conns.find(fd);
	assert(iter != _conns.end());//运行时断言
   // cout<<"121212"<<endl;
   // handleMessageCallback
   // TcpConnection
	if(!isClosed) {
		iter->second->handleMessageCallback();
	} else {
		delEpollFdRead(fd);
		iter->second->handleCloseCallback();
		_conns.erase(iter);
	}
    cout<<"131313"<<endl;
}
//还有这里
//没有进去handleRead
void EventLoop::handleRead()
{
    cout<<"141414"<<endl;
	uint64_t howmany;
	int ret = ::read(_eventfd, &howmany, sizeof(howmany));
    cout<<"161616"<<endl;
	if(ret != sizeof(howmany)) {
		perror("read");
	}
    cout<<"151515"<<endl;
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	int ret = ::write(_eventfd, &one, sizeof(one));
	if(ret != sizeof(one)) {
		perror("write");
	}
}

void EventLoop::doPendingFunctors()
{
	vector<Functor> tmp;
	{
		MutexLockGuard autolock(_mutex);
		tmp.swap(_pendingFunctors);
	}

	for(auto & functor : tmp)
	{
		functor();
	}
}

bool EventLoop::isConnectionClosed(int fd)
{
	int ret;
	do{
		char buff[1024];
		ret = recv(fd, buff, sizeof(buff), MSG_PEEK);
	}while(ret == -1 && errno == EINTR);

	return (ret == 0);
}
	
int EventLoop::createEpollFd()
{
	int ret = ::epoll_create1(0);
	if(ret == -1) {
		perror("epoll_create1");
	}
	return ret;
}

int EventLoop::createEventFd()
{
	int ret = ::eventfd(0, 0);
	if(ret == -1) {
		perror("eventfd");
	}
	return ret;
}

void EventLoop::addEpollFdRead(int fd)
{
	struct epoll_event evt;
	evt.data.fd = fd;
	evt.events = EPOLLIN;
	int ret = epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &evt);
	if(-1 == ret) {
		perror("epoll_ctl");
	}
}

void EventLoop::delEpollFdRead(int fd)
{
	struct epoll_event evt;
	evt.data.fd = fd;
	int ret = epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &evt); 
	if(-1 == ret) {
		perror("epoll_ctl");
	}
}
}//end of namespace hk

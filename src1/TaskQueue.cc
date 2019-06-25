
#include "TaskQueue.h"
#include <iostream>
using namespace std;

namespace hk
{

bool TaskQueue::full() const 
{
    return _que.size()==_queSize;
}

bool TaskQueue::empty() const
{
    return _que.empty(); 
}
//生产者线程生产一个任务 调用push 压入任务队列
void TaskQueue::push(const ElemType & elem)
{
    {
        MutexLockGuard autolock(_mutex);
        while(full())
        {
            _notFull.wait();//使用while是为了防止异常唤醒
            //等待一个不满的信号
        }
        _que.push(elem);
    }//使用语句块减小加锁的范围
    _notEmpty.notify();
    //每次生产一个任务 都发出队列不空的信号
}

void TaskQueue::wakeup()
{
    if(_flag)
        _flag=false;
    _notEmpty.notifyAll();
}


//消费者消费任务 调用这个pop方法
ElemType  TaskQueue::pop()
{
    ElemType ret;

    MutexLockGuard autolock(_mutex);
    while(_flag && empty())
    {
        //怎么解决这里线程可能无法退出的情况
        //如果任务队列空了 就要等待不空的信号来了
        //才会跳出循环 否则卡死在这里了。
        //notifyAll wakeup()
        _notEmpty.wait();
    }
    if(_flag)
    {
        ret=_que.front();
        _que.pop();
        _notFull.notify();
        return ret; 
    }
    else
    {
        return nullptr;//这里写成ElemType();???
    }
    //每次消费一个 都发出一个不满的信号
}


}//end of namespace hk

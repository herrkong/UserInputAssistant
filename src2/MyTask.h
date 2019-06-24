#pragma once
#include "MyDict.h"
#include "TcpConnection.h"
#include <queue>


namespace hk
{

struct MyResult
{
    string _word; //候选词
    int  _wordFreq;//词频
    int _wordDist;//与查询词的最小距离
};

class MyCompare
{
public:
    bool operator()(const MyResult & lhs,const MyResult & rhs)
    {
        if(lhs._wordDist != rhs._wordDist) 
            return lhs._wordDist >rhs._wordDist;
        else if(lhs._wordFreq != rhs._wordFreq)
            return lhs._wordFreq <rhs._wordFreq;
        else 
            return lhs._word > rhs._word;
    }
};



class MyTask
{
    using ResultQue = priority_queue<MyResult,vector<MyResult>,MyCompare>; 
public:
    MyTask(MyDict & dict,const string & queryWord,const TcpConnectionPtr & conn);
    void query();//执行查询
    void queryIndexTable();//查询索引
    void process(set<int> &  );//进行计算
    int  editdistance(const string &);//最小编辑距离
    void response(); //更新Cache



private:
    MyDict & _dict;   //词典
    string _queryWord; //查询词
    ResultQue _resultQue; //候选词队列   
    TcpConnectionPtr _conn; 
    //TcpConnection对象 建立连接 
    //将候选词发送给客户端
};


}//end of namespace hk

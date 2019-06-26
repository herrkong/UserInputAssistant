#pragma once
#include "TcpConnection.h"
#include "MyDict.h"
#include <queue>
#include <string> 
#include <set>
#include <json/json.h>
#include <sstream>

using namespace std;

namespace  hk
{

struct MyResult
{
    string _word; //候选词
    int _iFreq; //词频
    int _iDist; //与查询词的最小编辑距离

};

class MyCompare
{
public:
    bool operator()(const MyResult & lhs,const MyResult & rhs)
    {
        if(lhs._iDist != rhs._iDist) 
            return lhs._iDist >rhs._iDist;
        else if(lhs._iFreq != rhs._iFreq)
            return lhs._iFreq <rhs._iFreq;
        else 
            return lhs._word > rhs._word;
    }
};


using ResultQue = priority_queue<MyResult,vector<MyResult>,MyCompare>;


class MyTask
{
public:
    MyTask(MyDict & dict,const string & queryWord,const TcpConnectionPtr & conn); 
    void execute();//执行查询
    void queryIndexTable();//查询索引表
    void insertQueue(); //插入优先级队列
    void statistic(set<int> & iset);//进行计算

    int  distance(const string & rhs);//计算最小编辑距离
    void response(); //待发送的查询结果
    //void response(Cache & cache);

private:
    MyDict &_dict;//词典
    string _queryWord;   //待查询单词
    ResultQue _resultQue;  //保存候选结果集的优先级队列
    TcpConnectionPtr _conn;
    //TcpConnection对象 建立连接 将候选词发送给客户端
    set<int> _words;
};
}//end of namespace hk

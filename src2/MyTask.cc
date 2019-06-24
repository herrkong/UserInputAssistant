#include "MyTask.h"
#include "EditDistance.h"

using namespace std;

namespace hk
{

MyTask::MyTask(MyDict & dict,const string & queryWord,const TcpConnectionPtr & conn)
:_dict(dict)
,_conn(conn)
{
    _queryWord=queryWord.substr(0,queryWord.size()-1);
    //string类型的字符串以'\0'结尾。
    cout<<"MyTask(MyDict &,const string &,const TcpConnectionPtr & conn)"<<endl;
}

void MyTask::query()
{
    queryIndexTable();
    response(); 
}


void MyTask::queryIndexTable()
{
    for(size_t idx =0 ; idx != _queryWord.size() ; ++idx)
    {
        auto indexTable = _dict.getIndexTable();
        string ch = string(1,_queryWord[idx]);//把一个char变成string        
        process(indexTable[ch]);
    }
}

void MyTask::process(set<int> & myset)
{   
    auto dict = _dict.getDict();
    MyResult temp;

    
    for(auto iter = myset.begin();iter != myset.end();++iter )
    {
        temp._word = dict[*iter].first;
        temp._wordFreq = dict[*iter].second;
        temp._wordDist = editdistance(dict[*iter].first);
        if(temp._wordDist < 4 )//编辑距离小于4的压入队列 
        {
            _resultQue.push(temp);
        }    
    }
}


int MyTask::editdistance(const string & rhs)
{
   EditDistance  editdist(_queryWord,rhs);
   return editdist.get();
}


void MyTask::response()
{
    _conn->sendInLoop(_resultQue.top()._word);
    priority_queue<MyResult,vector<MyResult>,MyCompare> temp;
    _resultQue.swap(temp);//清空队列
}


}//end of namespace hk



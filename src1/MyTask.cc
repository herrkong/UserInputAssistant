#include "MyTask.h"
#include "EditDistance.h"

namespace  hk
{


MyTask::MyTask(MyDict & dict,const string & queryWord,const TcpConnectionPtr & conn)
:_dict(dict)
,_queryWord(queryWord)
,_conn(conn)
{
    cout<<"MyTask()"<<endl;
}

void MyTask::execute()
{
    queryIndexTable();//先检查索引 再计算编辑距离
    response();
}

void MyTask::statistic(set<int> & iset)
{

    auto dict = _dict.getDict();
    MyResult temp;

    for(auto iter = iset.begin();iter != iset.end();++iter )
    {
        temp._word = dict[*iter].first;
        temp._iFreq = dict[*iter].second;
        temp._iDist = distance(dict[*iter].first);
       
       // if(temp._iDist < 10 ) 
       // {
            _resultQue.push(temp);//问题在这里 
       // }
    
    }
}

void MyTask::queryIndexTable()
{
    for(size_t idx =0 ; idx != _queryWord.size() ; ++idx)
    {
        auto indexTable = _dict.getIndexTable();
        string ch = string(1,_queryWord[idx]);//把一个char变成string        
        statistic(indexTable[ch]);
    }
}

int MyTask::distance(const string & rhs)
{
    EditDistance editdist(_queryWord,rhs);
    return editdist.get();
}

void MyTask::response()
{   
    cout<<"before response"<<endl;
    //暂时只发送一个候选词
    cout<<"233"<<endl;
    cout<<_resultQue.top()._word<<endl;
    //失败时 这里都没打印出来 说明队列之中没有词 
    cout<<"666"<<endl;
    _conn->sendInLoop(_resultQue.top()._word);
    ResultQue empty;
    _resultQue.swap(empty);//清空队列
    cout<<"end response"<<endl;
}

}//end of namespace hk


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

    cout<<"进入statistic()"<<endl;
    auto dict = _dict.getDict();
    //为什么一旦失败 我连词典都没拿到。
    cout<<"得到词典了么？"<<endl;
    MyResult temp;
    
    //就没进入for循环
    for(auto iter = iset.begin();iter != iset.end();++iter )
    {
        cout<<"进入了for循环"<<endl;
        temp._word = dict[*iter].first;
        temp._iFreq = dict[*iter].second;
        temp._iDist = distance(dict[*iter].first);
       
       // if(temp._iDist < 10 ) 
       // {   
            cout<<"push-->"<<temp._word<<endl;
            _resultQue.push(temp);//问题在这里 
            cout<<"push-->"<<temp._word<<"finished"<<endl;    

        // }
        cout<<"cnm2"<<endl;
    
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
    if(_resultQue.empty())
    {
        _conn->sendInLoop("No match word");
    }
    else
    {
        cout<<_resultQue.top()._word<<endl;
        //失败时 这里都没打印出来 说明队列之中没有词 
        _conn->sendInLoop(_resultQue.top()._word);
        ResultQue empty;
        _resultQue.swap(empty);//清空队列
    }
}

}//end of namespace hk
